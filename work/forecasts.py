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
from pyspark.mllib.linalg import Vectors
from pyspark.ml.feature import VectorAssembler
import pyspark.sql.functions as f
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
        self.trand = 'n'
        self.deviation = 1
        self.index = None
        self.target = None
        self.by = None
        self.direct = 'both'
        self.alert_window = 'last_60s'
        self.seasonality = 'houly'
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

        if self.seasonality == 'monthly':
            self.seasonality = 12
        elif self.seasonality == 'weekly':
            self.seasonality = 7
        else:
            self.seasonality = 24

    def dfToPandas(self, df) :
        data = df.toPandas()
        data[self.index]  = data[self.index].astype(str)
        data[self.index]  = pd.to_datetime(data[self.index],format='%Y%m%d%H%M%S')
        data[self.target] = data[self.target].astype(float)
        data = data.set_index(self.index)
        return data

    def linear(self, sqlCtx, df):
        '''
        Forecasts using linear regression model

        Args:
            df: dataframe.
        Returns:
            The forecasts dataframe.
            
        '''
        data = df.toPandas()
        data[self.index]  = pd.to_datetime(data[self.index],format='%Y%m%d%H%M%S')
        data = data.sort_values(by=[self.index])

        count_index = []
        for i in range(len(data)):
            count_index.append(i)
        data.insert(0,'index',count_index)

        data['forecasts'] = False

        last_data = data['index'].iloc[-1]
        interval = data['index'].iloc[1] - data['index'].iloc[0]
        new_features = []
        tmp = last_data + interval
        for i in range(len(data)):
            new_features.append(tmp)
            tmp = tmp + interval

        new_time = []
        last_data = data[self.index].iloc[-1]
        interval = data[self.index].iloc[1] - data[self.index].iloc[0]
        tmp = last_data + interval
        for i in range(len(data)):
            new_time.append(tmp)
            tmp = tmp + interval

        test = data
        test = test.drop(['index',self.target],axis=1)
        test['forecasts'] = True
        test.insert(0, "index", new_features)

        test = test.drop([self.index],axis=1)
        test.insert(1, self.index, new_time)
        data[self.index]  = data[self.index].astype(str)
        test[self.index]  = test[self.index].astype(str)
        
        df = sqlCtx.createDataFrame(data)
        p_df = sqlCtx.createDataFrame(test)
        assembler = VectorAssembler(inputCols=['index'], outputCol='features')
        df = assembler.transform(df)
        p_df = assembler.transform(p_df)

        # 선형회귀 모델을 생성합니다.
        # maxIter : 최적화 알고리즘의 최대 수행 횟수입니다.
        # regParam : 정규호 파라미터입니다. 예측에서는 크게 변경 가능성이 없습니다.
        # labelCol : 학습을 진행할 정답 열을 지정해줍니다.
        lr = LinearRegression(maxIter=100,regParam=0.1,featuresCol='features',labelCol=self.target)
        lrModel = lr.fit(df)

        prediction = lrModel.transform(p_df)
        prediction = prediction.select(prediction['index'],prediction[self.index],prediction['prediction'],prediction['forecasts'])

        # 원본 데이터와 예측 데이터를 합치기 위해서 열의 이름을 똑같이 바꿔줍니다.
        prediction = prediction.selectExpr("index as index",self.index+" as "+self.index, "prediction as "+self.target, "forecasts as forecasts")
        
        df = df.select(df['index'],df[self.index],df[self.target],df['forecasts'])

        result = df.union(prediction)
        
        return result

    def seasonal(self, sqlCtx, df) :
    	'''
    	Forecasts using SARIMA model

    	Args:
    		df: dataframe.
    	Returns:
    		The forecasts dataframe.
    	Note:
    		To need compute P,Q,M parameters by yourself.
    	'''

        data = self.dfToPandas(df)
        data = data.sort_values(by=[self.index])
        data['forecasts'] = False
        data['lower '+self.target] = 0
        data['upper '+self.target] = 0

        # SARIMAX 모델을 생성합니다.
        # trand : 추세를 고려하는 방법을 설정하는 변수 참조. (https://www.statsmodels.org/dev/generated/statsmodels.tsa.statespace.sarimax.SARIMAX.html)
        # order : p,d,q - 자동회귀, 통합, 이동평균 계수 설정. (https://www.statsmodels.org/dev/generated/statsmodels.tsa.statespace.sarimax.SARIMAX.html)
        # seasonal_order : 위와 동일하고 마지막 계절성 파라미터가 추가됩니다. 주기와 관련되 파라미터 설정. (https://www.statsmodels.org/dev/generated/statsmodels.tsa.statespace.sarimax.SARIMAX.html)
        mod_sarimax = sm.tsa.SARIMAX(data[self.target],trand=self.trand, order=(0,1,0),seasonal_order=(1,1,1,self.seasonality))
        res_sarimax = mod_sarimax.fit()

        new_time = []
        last_data = data.index[-1]
        interval = data.index[1] - data.index[0]
        tmp = last_data + interval
        for i in range(len(data)):
            new_time.append(tmp)
            tmp = tmp + interval

        test = data
        test = test.drop([self.target],axis=1)
        test[self.index] = new_time
        test['forecasts'] = True

        # sparkdataframe으로 바꾸기 위해서 time index값을 열로 추가.
        data.reset_index(inplace = True)
        
        # N : 예측 데이터 크기.
        # 생성된 모델을 가지고 예측 값과 예측 값의 신뢰구간을 저장합니다.
        N = len(data)
        pred_uc = res_sarimax.get_forecast(steps=N)
        pred_ci = pred_uc.conf_int()
        test[self.target] = pred_uc.predicted_mean.values
        test['lower '+self.target] = pred_ci['lower '+self.target].values*self.deviation
        test['upper '+self.target] = pred_ci['upper '+self.target].values*self.deviation
        test = test.round({self.target:2,'lower '+self.target:2,'upper '+self.target:2})

        # spark dataframe으로 바꿀시 datetime의 단위가 이상하게 바뀌므로 string으로 변환합니다.
        data[self.index]  = data[self.index].astype(str)
        test[self.index] = test[self.index].astype(str)

        df = sqlCtx.createDataFrame(data)
        p_df = sqlCtx.createDataFrame(test)
        df = df.select(df[self.index],df[self.target],df['lower '+self.target],df['upper '+self.target],df['forecasts'])
        p_df = p_df.select(p_df[self.index],p_df[self.target],p_df['lower '+self.target],p_df['upper '+self.target],p_df['forecasts'])
        result = df.union(p_df)

        return result

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

