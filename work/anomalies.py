# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from angora.cmds.base import SimpleCommand
from angora.exceptions import AngoraException
from ply import lex, yacc
from pyspark.sql.types import *
from pandas import Series
import pandas as pd
import statsmodels.api as sm
from statsmodels.tsa.seasonal import seasonal_decompose
import math

class anomaliesCommand(SimpleCommand):

    SUPPORT_MODE = ["basic", "agile", "robust"]
    EXCEPTION_01 = "We only support one of " + str(SUPPORT_MODE) + " anomaly algorithm."
    
    class anomaliesLexer(object):

        tokens = (
            'WORD',
            'DOUBLE',
            'EQUALS',
        )

        # Regular expression rules for simple tokens
        t_WORD = r'\w+'
        t_ignore = ' \t'
        t_EQUALS = r'\='

        def t_newline(self, t):
            r'\n+'
            t.lexer.lineno += len(t.value)

        def t_DOUBLE(self, t):
            '[-+]?[0-9]+(\.([0-9]+)?([eE][-+]?[0-9]+)?|[eE][-+]?[0-9]+)'

        def t_error(self, t):
            raise SyntaxError("Syntax error near '%s'" % t.value)

        def build(self, **kwargs):
            self.lexer = lex.lex(module=self, **kwargs)
            return self.lexer

    class anomaliesParser(object):

        EXCEPTION_PARSE = "eval has errors."
        COUNT = 1

        tokens = (
            'WORD',
            'DOUBLE',
            'EQUALS',
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
                     | WORD EQUALS DOUBLE
                     '''
            if len(p) == 4:
                if p[3].lower() =="true":
                    p[3] = True
                elif p[3].lower() =="false":
                    p[3] = False
                p[0] = [p[1],p[3]]
            else:
                p[0] = [p[1],p[4]]

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
        self.parse_parameter = []

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
            raise AngoraException(anomaliesCommand.EXCEPTION_03)

        if "target" in parsed_args:
            self.target = parsed_args['target']
        else:
            raise AngoraException(anomaliesCommand.EXCEPTION_03)

        self.parse_parameter = parsed_args['params']
        par_list = ['alg', 'bound']

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
		data = data.set_index(self.index)
		data.index = pd.to_datetime(data.index)
		data = data.astype(float)

		return data

    def basic(self, df) :
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
        upper = ma + confidence
        lower = ma - confidence

        data.insert(len(data.columns),"upper",upper)
        data.insert(len(data.columns),"lower",lower)

        return data[(data[self.target] > upper) | (data[self.target] < lower)]

    def agile(self, df) :
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

        mod_sarimax = sm.tsa.SARIMAX(data, order=(1,1,1),seasonal_order=(1,1,1,4))
        res_sarimax = mod_sarimax.fit()

        N = len(data)
        pred_results = res_sarimax.predict(start=data.index.get_loc(data.index[0]),end=data.index.get_loc(data.index[N-1]))
        print data
        print pred_results

        pred_differences = data.value-pred_results
        pred_differences = abs(pred_differences)

        ma5 = pred_differences.rolling(window=5,min_periods=1).mean()
        stddev5 = pred_differences.rolling(window=5,min_periods=1).std()

        confidence_interval_upper = ma5 + ((1.959964*stddev5*self.bound)/math.sqrt(5))
        confidence_interval_lower = ma5 - ((1.959964*stddev5*self.bound)/math.sqrt(5))

        data.insert(len(data.columns), "predict", pred_results)
        data.insert(len(data.columns), "residuals", pred_differences)
        data.insert(len(data.columns), "upper", confidence_interval_upper)
        data.insert(len(data.columns), "lower", confidence_interval_lower)

        print(res_sarimax.summary())

        return data[(data['residuals'] > confidence_interval_upper) | (data['residuals'] < confidence_interval_lower)]

    def robust(self, df) :
    	'''
    	Detect anomailes using Seasonal Trend decomposition

    	Args:
    		df: dataframe.
    	Returns:
    		The anomailies dataframe.
    	Note:
    		
    	'''

    	data = self.dfToPandas(df)
    	result = seasonal_decompose(data,model='multiplicative', freq=2)
    	# print result.seasonal
    	# result.seasonal = math.log(result.seasonal['value'])
    	# print result.seasonal
    	resid = result.resid

    	ma5 = resid.rolling(window=5,min_periods=1).mean()
    	stddev5 = resid.rolling(window=5,min_periods=1).std()
    	
        confidence = 1.959964 * stddev5 * self.bound
        print 'confidence' + str(confidence)
    	upper = ma5 + (confidence/math.sqrt(5))
    	lower = ma5 - (confidence/math.sqrt(5))

    	data.insert(len(data.columns), "residuals", resid)
    	data.insert(len(data.columns), "upper", confidence_interval_upper)
    	data.insert(len(data.columns), "lower", confidence_interval_lower)

    	return data[ (data['residuals'] > confidence_interval_upper['value']) |
    				 (data['residuals'] < confidence_interval_lower['value']) ]

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
            return self.basic(df)
        elif self.alg == "agile":
            return self.agile(df)
        elif self.alg == "robust":
            return self.robust(df)
        else:
            raise AngoraException(anomaliesCommand.EXCEPTION_01)

