# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from angora.cmds.base import SimpleCommand
from angora.exceptions import AngoraException
from ply import lex, yacc
from pyspark.sql.types import *
from pandas import Series
import pandas as pd
import math
import sys
from sklearn.cluster import DBSCAN
from sklearn.preprocessing import StandardScaler

class outlierCommand(SimpleCommand):

    SUPPORT_MODE = ["dbscan", "mad"]
    EXCEPTION_01 = "We only support one of " + str(SUPPORT_MODE) + " outlier algorithm."
    EXCEPTION_05 = "The mad algorithm not need pct parameter."
    EXCEPTION_06 = "by=??? need a group columns"
    EXCEPTION_07 = "pct value must lower than 1 or moust upper than 0"
    
    class outlierLexer(object):

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

    class outlierParser(object):

        EXCEPTION_PARSE = "eval has errors."

        tokens = (
            'WORD',
            'NUMBER',
            'EQUALS',
            'DOT',
        )

        def p_outlier_command(self, p):
            'outlier_command : target params'
            p[0] = {'target':p[1], 'params':p[2]}

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

        super(outlierCommand, self).__init__()
        self.lexer = outlierCommand.outlierLexer().build()
        self.parser = outlierCommand.outlierParser().build(write_tables=False, debug=False)
        
        self.alg = 'dbscan'
        self.index = None
        self.target = None
        self.tolerance = 0.3
        self.pct = 0.1
        self.by = None
        self.parse_parameter = []
        self.C = 0
        self.S = []

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
            e.message = "%s [outlier %s]" % (e.message, raw_args)
            raise e

    def parsing_and_exception(self,parsed_args):
        if "target" in parsed_args:
            self.target = parsed_args['target']
        else:
            raise AngoraException(outlierCommand.EXCEPTION_03)

        self.parse_parameter = parsed_args['params']
        par_list = ['alg', 'tolerance', 'pct', 'by']

        if self.parse_parameter != None:
            for i in par_list:
                try:
                    if i in self.parse_parameter:
                        run = "self." +i + "= " + "self.parse_parameter" + "['"+ i +"']"
                        exec(run)
                except:
                    pass

        if self.alg == 'dbscan':
            if "pct" in self.parse_parameter:
                raise AngoraException(outlierCommand.EXCEPTION_05)

        if self.by == None:
            raise AngoraException(outlierCommand.EXCEPTION_06)

        if self.pct > 1 or self.pct <=0:
            raise AngoraException(outlierCommand.EXCEPTION_07)

    def dbscan(self, sqlCtx, df) :
    	'''
    	Detect outlier using basic argorithm.

    	Args:
    		df: dataframe.
    	Returns:
    		the outlier dataframe.
    	Note:
    		Need revise this algorithm.
    	'''
    	data = df.toPandas()
        X = pd.DataFrame(data[self.target])
        X = StandardScaler().fit_transform(X)

        # 밀도기반 클러스터링 모델을 생성합니다.
        model = DBSCAN().fit(X)
        predict = model.fit_predict(X)
        data['label'] = predict
        data['outlier'] = False
        cluster = data['label'].unique()
        
        # 군집화된 클러스터에서 검출된 host가 만약 1개라면
        # 그 호스트는 모두 아웃라이어입니다.
        # 이상한 호스트가 아닌 이상 군집화 클러스터에 여러 호스트가 함께 존재해야합니다.
        result= []
        for label in cluster:
            tmp = data[data['label']==label]
            host_col_list = tmp[self.by].unique()
            host_count = len(host_col_list)
            if host_count == 1:
                tmp = data[data[self.by]==host_col_list[0]]
                tmp['outlier'] = True
            result.append(tmp)
        data = pd.concat(result)
        data = data.astype(str)
        st = []
        for col in data.columns:
            if(data[col].dtype == 'float64'):
                st.append(StructField(col,DoubleType()))
            elif(data[col].dtype == 'bool'):
                st.append(StructField(col,BooleanType()))
            elif(data[col].dtype == 'int64'):
                st.append(StructField(col,IntegerType()))
            else:
                st.append(StructField(col,StringType()))
        schema = StructType(st)
        df = sqlCtx.createDataFrame(data, schema)
        return df

    def mad(self, sqlCtx, df):
        data = df.toPandas()
        X = pd.DataFrame(data[self.target])
        data['outlier'] = False

        # 들어오는 호스트별로 각각의 mad(중앙값 절대 편차)를 계산합니다.
        # 중앙값 절대 편차를 넘어서는 값을 이상 값으로 간주합니다.
        # 이상값이 전체 데이터*pct 갯수 만큼 검출될 시 host를 이상하다고 판단합니다.
        group_columns = data[self.by].unique()
        result = []
        for group in group_columns:
            tmp = data[data[self.by]==group]
            median = tmp[self.target].median()
            mad = abs(tmp[self.target] - median).median()
            confidence = 1.959964 * mad * float(self.tolerance)
            tmp['upper'] = tmp[self.target] + confidence
            tmp['lower'] = tmp[self.target] - confidence
            tmp['anomaly'] = (tmp[self.target] > tmp['upper']) | (tmp[self.target] < tmp['lower'])

            count = len(tmp[tmp['anomaly']==True])
            if count > len(tmp)*self.pct:
                tmp['outlier'] = True
            else:
                tmp['outlier'] = False
            result.append(tmp)
        data = pd.concat(result)
        data = data.round({'lower':2,'upper':2})
        data = data.astype(str)
        st=[]
        for col in data.columns:
            if(data[col].dtype == 'float64'):
                st.append(StructField(col,DoubleType()))
            elif(data[col].dtype == 'bool'):
                st.append(StructField(col,BooleanType()))
            elif(data[col].dtype == 'int64'):
                st.append(StructField(col,IntegerType()))
            else:
                st.append(StructField(col,StringType()))
        schema = StructType(st)
        df = sqlCtx.createDataFrame(data, schema)
        df = df.select(df[self.by],df[self.target],df['lower'],df['upper'],df['anomaly'],df['outlier'])
        df.show()
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

        if self.alg == "dbscan":
            return self.dbscan(sqlCtx, df)
        elif self.alg == "mad":
            return self.mad(sqlCtx, df)
        else:
            raise AngoraException(outlierCommand.EXCEPTION_01)

