# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.sql.types import StructType, StructField, IntegerType, StringType
from angora.cmds.base import SimpleCommand
from angora.exceptions import AngoraException
from ply import lex, yacc
from pyspark.sql.types import *
from pandas import Series
import pandas as pd
import statsmodels.api as sm
from statsmodels.tsa.seasonal import seasonal_decompose
import math
import sys

class anomaliesCommand(SimpleCommand):

    SUPPORT_MODE = ["basic", "agile", "robust"]
    EXCEPTION_01 = "We only support one of " + str(SUPPORT_MODE) + " anomaly algorithm."
    EXCEPTION_05 = "Alert_winsize is out of dataframe size"
    
    class anomaliesLexer(object):

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
            '[-+]?[0-9]+(\.([0-9]+)?([eE][-+]?[0-9]+)?|[eE][-+]?[0-9]+)'

        # def t_DOUBLE(self, t):
        #     '[+-]?(([1-9][0-9]*)|(0))([.,][0-9]+)?'

        def t_error(self, t):
            raise SyntaxError("Syntax error near '%s'" % t.value)

        def build(self, **kwargs):
            self.lexer = lex.lex(module=self, **kwargs)
            return self.lexer

    class anomaliesParser(object):

        EXCEPTION_PARSE = "eval has errors."

        tokens = (
            'WORD',
            'NUMBER',
            'EQUALS',
            'DOT',
        )

        def p_anomalies_command(self, p):
            'anomalies_command : index target params'
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
                     | WORD EQUALS NUMBER DOT NUMBER
                     '''
            if len(p) == 4:
                p[0] = [p[1],p[3]]
            else:
                eprint ('asdfasdfasdfasdf')
                p[0] = [p[1],p[3]+p[4]+p[5]]

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

        super(anomaliesCommand, self).__init__()
        self.lexer = anomaliesCommand.anomaliesLexer().build()
        self.parser = anomaliesCommand.anomaliesParser().build(write_tables=False, debug=False)
        
        self.alg = 'basic'
        self.bound = 2
        self.index = None
        self.target = None
        self.direct = 'both'
        self.window = 0.1
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
            e.message = "%s [anomalies %s]" % (e.message, raw_args)
            raise e

    def parsing_and_exception(self,parsed_args):

        if "index" in parsed_args:
            self.index = parsed_args['index']
        else:
            raise AngoraException(anomaliesCommand.EXCEPTION_02)

        if "target" in parsed_args:
            self.target = parsed_args['target']
        else:
            raise AngoraException(anomaliesCommand.EXCEPTION_03)

        self.parse_parameter = parsed_args['params']
        par_list = ['alg', 'bound', 'direct', 'window']
        detect_list = ['direct', 'window']

        if self.parse_parameter != None:
            for i in detect_list:
                if i in self.parse_parameter:
                    self.detection_sign = True
                    break

            for i in par_list:
                try:
                    if i in self.parse_parameter:
                        run = "self." +i + "= " + "self.parse_parameter" + "['"+ i +"']"
                        exec(run)
                except:
                    pass

    def dfToPandas(self, df) :
        data = df.toPandas()
    	data = data.set_index(self.index)
        data = data.astype(float)
    	data.index = pd.to_datetime(data.index)

    	return data

    def basic(self, sqlCtx, df) :
    	'''
    	Detect anomalies using basic argorithm.

    	Args:
    		df: dataframe.
    	Returns:
    		the anomalies dataframe.
    	Note:
    		Need revise this algorithm.
    	'''
    	data = self.dfToPandas(df)
        window = 5

        ma = data[self.target].rolling(window=window,min_periods=1).mean()
        all_std = data[self.target].std()
        stddev = all_std/math.sqrt(5)
        confidence = 1.959964 * stddev * float(self.bound)

        if self.detection_sign :
            self.window = float(self.window)
            if (self.window > 0) and (self.window <= 1):
                alert_count = data.shape[0] * self.window
            elif (self.window <= 0) or (self.window > data.shape[0]):
                raise AngoraException(anomaliesCommand.EXCEPTION_05)
            else :
                alert_count = self.window
            
            ma = ma.iloc[int(data.shape[0]-alert_count):]
            data = data.iloc[int(data.shape[0]-alert_count):]

        upper = ma + confidence
        lower = ma - confidence

        data.insert(len(data.columns),"upper",upper)
        data.insert(len(data.columns),"lower",lower)
        data.reset_index(inplace = True)
        data[self.index] = data[self.index].astype(str)

        if self.direct == 'below':
            anomaly = data[self.target] < data['lower']
        elif self.direct == 'above':
            anomaly = data[self.target] > data['upper']
        else:
            anomaly = (data[self.target] > data['upper']) | (data[self.target] < data['lower'])
        
        data.insert(len(data.columns),"anomaly",anomaly)

        try:
            df = sqlCtx.createDataFrame(data)
        except:
            schema = StructType([ 
                StructField("time", DoubleType(), nullable=False),
                StructField("value", DoubleType(), nullable=False),
                StructField("upper", DoubleType(), nullable=False),
                StructField("lower", DoubleType(), nullable=False),
                StructField("anomaly", DoubleType(), nullable=False),])
            df = sqlCtx.createDataFrame(data,schema)

        return df

    def agile(self, sqlCtx, df) :
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

        mod_sarimax = sm.tsa.SARIMAX(data, order=(1,0,0),seasonal_order=(0,0,0,0),enforce_stationarity=False)
        res_sarimax = mod_sarimax.fit()

        N = len(data)
        pred_results = res_sarimax.predict(start=data.index.get_loc(data.index[0]),end=data.index.get_loc(data.index[N-1]))

        pred_differences = data.value-pred_results
        pred_differences = abs(pred_differences)

        ma = pred_differences.rolling(window=5,min_periods=1).mean()
        all_std = pred_differences.std()
        stddev = all_std/math.sqrt(5)
        confidence = 1.959964 * stddev * float(self.bound)

        if self.detection_sign :
            self.window = float(self.window)
            if (self.window > 0) and (self.window <= 1):
                alert_count = data.shape[0] * self.window
            elif (self.window <= 0) or (self.window > data.shape[0]):
                raise AngoraException(anomaliesCommand.EXCEPTION_05)
            else :
                alert_count = self.window
            ma = ma.iloc[int(data.shape[0]-alert_count):]
            data = data.iloc[int(data.shape[0]-alert_count):]

        upper = ma + confidence
        lower = ma - confidence

        data.insert(len(data.columns), "predict", pred_results)
        data.insert(len(data.columns), "residuals", pred_differences)
        data.insert(len(data.columns), "upper", upper)
        data.insert(len(data.columns), "lower", lower)
        data.reset_index(inplace = True)
        data[self.index] = data[self.index].astype(str)

        if self.direct == 'below':
            anomaly = data['residuals'] < data['lower']
        elif self.direct == 'above':
            anomaly = data['residuals'] > data['upper']
        else:
            anomaly = (data['residuals'] > data['upper']) | (data['residuals'] < data['lower'])

        data.insert(len(data.columns),"anomaly",anomaly)

        if self.detection_sign :
            data = data.loc[data['anomaly'] == True]

        try:
            df = sqlCtx.createDataFrame(data)
        except:
            schema = StructType([ 
                StructField("time", DoubleType(), nullable=False),
                StructField("value", DoubleType(), nullable=False),
                StructField("predict", DoubleType(), nullable=False),
                StructField("residuals", DoubleType(), nullable=False),
                StructField("upper", DoubleType(), nullable=False),
                StructField("lower", DoubleType(), nullable=False),
                StructField("anomaly", DoubleType(), nullable=False),])
            df = sqlCtx.createDataFrame(data,schema)

        return df

    def robust(self, sqlCtx, df) :
    	'''
    	Detect anomailes using Seasonal Trend decomposition

    	Args:
    		df: dataframe.
    	Returns:
    		The anomailies dataframe.
    	Note:
    		
    	'''

    	data = self.dfToPandas(df)
    	result = seasonal_decompose(data,freq=4,model='multiplicative')
    	# result.seasonal = math.log(result.seasonal['value'])
    	# print result.seasonal
    	resid = result.resid

    	ma = resid.rolling(window=5,min_periods=1).mean()
    	all_std = resid.std()
        stddev = all_std/math.sqrt(5)
        confidence = 1.959964 * stddev * float(self.bound)
        if self.detection_sign :
            self.window = float(self.window)
            if (self.window > 0) and (self.window <= 1):
                alert_count = data.shape[0] * self.window
            elif (self.window <= 0) or (self.window > data.shape[0]):
                raise AngoraException(anomaliesCommand.EXCEPTION_05)
            else :
                alert_count = self.window
            ma = ma.iloc[int(data.shape[0]-alert_count):]
            data = data.iloc[int(data.shape[0]-alert_count):]

        upper = ma + confidence
        lower = ma - confidence

    	data.insert(len(data.columns), "residuals", resid)
    	data.insert(len(data.columns), "upper", upper)
    	data.insert(len(data.columns), "lower", lower)
        data.reset_index(inplace = True)
        data[self.index] = data[self.index].astype(str)

        if self.direct == 'below':
            anomaly = data['residuals'] < data['lower']
        elif self.direct == 'above':
            anomaly = data['residuals'] > data['upper']
        else:
            anomaly = (data['residuals'] > data['upper']) | (data['residuals'] < data['lower'])

        data.insert(len(data.columns),"anomaly",anomaly)

        if self.detection_sign :
            data = data.loc[data['anomaly'] == True]

        try:
            df = sqlCtx.createDataFrame(data)
        except:
            schema = StructType([ 
                StructField("time", DoubleType(), nullable=False),
                StructField("value", DoubleType(), nullable=False),
                StructField("residuals", DoubleType(), nullable=False),
                StructField("upper", DoubleType(), nullable=False),
                StructField("lower", DoubleType(), nullable=False),
                StructField("anomaly", DoubleType(), nullable=False),])
            df = sqlCtx.createDataFrame(data,schema)

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

        if self.alg == "basic":
            return self.basic(sqlCtx, df)
        elif self.alg == "agile":
            return self.agile(sqlCtx, df)
        elif self.alg == "robust":
            return self.robust(sqlCtx, df)
        else:
            raise AngoraException(anomaliesCommand.EXCEPTION_01)

