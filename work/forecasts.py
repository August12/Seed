# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.sql.types import *
from angora.cmds.base import SimpleCommand
from angora.exceptions import AngoraException
from ply import lex, yacc
from pyspark.sql.types import *
from pandas import Series
import pandas as pd
import statsmodels.api as sm
from statsmodels.tsa.seasonal import seasonal_decompose
from datetime import timedelta
from pyspark.ml.regression import LinearRegression
from pyspark.ml.linalg import Vectors
import math
import sys

class forecastsCommand(SimpleCommand):

    SUPPORT_MODE = ["linear", "seasonal"]
    EXCEPTION_01 = "We only support one of " + str(SUPPORT_MODE) + " forecasts algorithm."
    EXCEPTION_05 = "Alert_window unit must have been h, m or s"
    EXCEPTION_06 = "Direct must have been both, above or below"
    
    class forecastsLexer(object):

        tokens = (
            'WORD',
            'NUMBER',
            'EQUALS',
            'DOT',
        )

        # Regular expression rules for simple tokens
        t_WORD = r'\w+'
        t_ignore = ' \t'
        t_EQUALS = r'\='
        t_DOT = r'\.'

        def t_newline(self, t):
            r'\n+'
            t.lexer.lineno += len(t.value)

        def t_NUMBER(self, t):
            r'\d+'
            t.value = int(t.value)
            return t

        # def t_DOUBLE(self, t):
        #     '[+-]?(([1-9][0-9]*)|(0))([.,][0-9]+)?'

        def t_error(self, t):
            raise SyntaxError("Syntax error near '%s'" % t.value)

        def build(self, **kwargs):
            self.lexer = lex.lex(module=self, **kwargs)
            return self.lexer

    class forecastsParser(object):

        EXCEPTION_PARSE = "eval has errors."

        tokens = (
            'WORD',
            'NUMBER',
            'EQUALS',
            'DOT',
        )

        def p_forecasts_command(self, p):
            'forecasts_command : index target params'
            p[0] = {'index':p[1], 'target':p[2], 'params': p[3]}

        def p_index(self, p):
            'index : WORD'
            p[0] = p[1]

        def p_target(self, p):
            'target : WORD'
            p[0] = p[1]

        def p_params(self,p) :
            '''params : param
                      | params param
                      |'''
            if len(p)>1:
                if type(p[1]) is list:
                    p[1] = {p[1][0] : p[1][1]}
                if len(p) >2:
                    for i in range(2,len(p)):
                        p[1][p[i][0]] = p[i][1]
                p[0] = p[1]
            else:
                p[0] = None

        def p_param(slef,p) :
            '''param : WORD EQUALS WORD
                     | WORD EQUALS NUMBER
                     | WORD EQUALS double
                     '''
            p[0] = [p[1],p[3]]

        def p_double(self,p) :
            'double : NUMBER DOT NUMBER'
            p[0] = float(str(p[1])+str(p[2])+str(p[3]))

        def p_error(self, p):
            if p is None:
                raise AngoraException(self.EXCEPTION_PARSE)
            else:
                raise SyntaxError("Syntax error near '%s'" % p.value)

        def build(self, **kwargs):
            self.parser = yacc.yacc(module=self, **kwargs)
            return self.parser

    def __init__(self):
        """do building up Lex and Yacc"""

        super(forecastsCommand, self).__init__()
        self.lexer = forecastsCommand.forecastsLexer().build()
        self.parser = forecastsCommand.forecastsParser().build(write_tables=False, debug=False)
        
        self.alg = 'linear'
        self.deviation = 2
        self.index = None
        self.target = None
        self.by = None
        self.direct = 'both'
        self.alert_window = 'last_60s'
        self.seasonality = 'hourly'
        self.parse_parameter = []
        self.detection_sign = False

    def parse(self, raw_args, options=None):
        '''사용자로부터 입력받은 raw_args 를 Lex, yacc parser 로 문법에 맞게 반환한다.

         사용자가 입력한 '검색어'를 Lex, Yacc parser 로 정해진 규격에 맞게 parsing 합니다. 사용자가 입력한 검색어의
        문법에 문제가 없을 시 반환되어 해당 명령어('eval')의 실행 함수로 전달됩니다.

        keyword arguments:
        raw_args -- 사용자가 입력한 명령어를 넘겨받은 raw arguments 입니다.

        Returns: Dictionary
        '''

        try:
            return self.parser.parse(raw_args, lexer=self.lexer)
        except SyntaxError as e:
            e.message = "%s [forecasts %s]" % (e.message, raw_args)
            raise e

    def parsing_and_exception(self,parsed_args):

        if "index" in parsed_args:
            self.index = parsed_args['index']
        else:
            raise AngoraException(forecastsCommand.EXCEPTION_02)

        if "target" in parsed_args:
            self.target = parsed_args['target']
        else:
            raise AngoraException(forecastsCommand.EXCEPTION_03)

        self.parse_parameter = parsed_args['params']
        par_list = ['alg', 'deviation', 'seasonality', 'by']

        if self.parse_parameter != None:
            for i in par_list:
                try:
                    if i in self.parse_parameter:
                        run = "self." +i + "= " + "self.parse_parameter" + "['"+ i +"']"
                        exec(run)
                except:
                    pass

    def dfToPandas(self, df) :
        data = df.toPandas()
        data[self.index]  = data[self.index].astype(str)
        data[self.index]  = pd.to_datetime(data[self.index],format='%Y%m%d%H%M%S')
        data[self.target] = data[self.target].astype(float)
        data = data.set_index(self.index)
        return data

    def linear(self, sqlCtx, df):
        
        lr = LinearRegression(maxIter=10, regParam=0.3, elasticNetParam=0.8,featuresCol='time',labelCol='value')
        lrModel = lr.fit(df)
        testData = pd.to_datetime([20181029192500,20181029192510,20181029192520,20181029192530])
        predictions = model.transform(testData)
        print predictions


    def sarimaAlgorithm(self, sqlCtx, data):
        time_tri = self.alert_window[len(self.alert_window)-1]
        alert_window_param = int(self.alert_window[5:len(self.alert_window)-1])

        mod_sarimax = sm.tsa.SARIMAX(data.values, order=(0,1,0),seasonal_order=(0,0,0,12))
        res_sarimax = mod_sarimax.fit()

        N = len(data)
        test = pd.to_datetime([20181029192500,20181029192510,20181029192520,20181029192530])
        pred_results = res_sarimax.predict(start=test[0],end=test[-1])

        print pred_results

        data.reset_index(inplace = True)
        data[self.index] = data[self.index].astype(str)
        return data

    def seasonal(self, sqlCtx, df) :
    	'''
    	Detect anomailes using SARIMA model

    	Args:
    		df: dataframe.
    	Returns:
    		The anomailies dataframe.
    	Note:
    		To need compute P,Q,M parameters.
    		This code has a critical problem. It is that pred_results push one by one.
    		***median absolute deviation***
    	'''

        data = self.dfToPandas(df)

        if self.by != None:
            group = self.by
            group_columns = data[group].unique()
            group_result = []
            for col in group_columns:
                tmp = data[data[group]==col]
                tmp = tmp.filter(items=[self.target])
                tmp = self.sarimaAlgorithm(sqlCtx, tmp)
                tmp[group] = col
                group_result.append(tmp)
            result_agile = pd.concat(group_result)
            print result_agile
        else:
            tmp = data.filter(items=[self.target])
            result_agile = self.sarimaAlgorithm(sqlCtx, tmp)

        st = []
        print result_agile.dtypes
        for col in result_agile.columns:
            if(result_agile[col].dtype == 'float64'):
                st.append(StructField(col,DoubleType()))
            elif(result_agile[col].dtype == 'bool'):
                st.append(StructField(col,BooleanType()))
            elif(result_agile[col].dtype == 'int64'):
                st.append(StructField(col,IntegerType()))
            else:
                st.append(StructField(col,StringType()))
        schema = StructType(st)
        df = sqlCtx.createDataFrame(result_agile, schema)

        return df

    def execute(self, sqlCtx, df=None, parsed_args=None, options=None):
        '''앞서 생성한 함수를 input data frame 에 적용합니다.

        keyword arguments:
        sqlCtx -- angora 에서 넘겨받은 spark SQL context.
        df -- 최초 넘겨받은 input data frame 이자 본 명령어를 적용하는 target data frame 입니다.
        parsed_args -- 사용자가 입력한 명령어 및 옵션이 parse 함수를 거쳐 parsing 된 arguments 입니다.

        Returns: DataFrame
        '''

        self.parsing_and_exception(parsed_args)

        if self.alg == "linear":
            return self.linear(sqlCtx, df)
        elif self.alg == "seasonal":
            return self.seasonal(sqlCtx, df)
        else:
            raise AngoraException(forecastsCommand.EXCEPTION_01)

