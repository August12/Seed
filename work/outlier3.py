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

class outlierCommand(SimpleCommand):

    SUPPORT_MODE = ["dbscan", "mad"]
    EXCEPTION_01 = "We only support one of " + str(SUPPORT_MODE) + " outlier algorithm."
    
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
            'outlier_command : index target params'
            p[0] = {'index':p[1], 'target':p[2], 'params':p[3]}

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

        super(outlierCommand, self).__init__()
        self.lexer = outlierCommand.outlierLexer().build()
        self.parser = outlierCommand.outlierParser().build(write_tables=False, debug=False)
        
        self.alg = 'dbscan'
        self.index = None
        self.target = None
        self.tolerance = 50.0
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
        if "index" in parsed_args:
            self.index = parsed_args['index']
        else:
            raise AngoraException(outlierCommand.EXCEPTION_03)

        if "target" in parsed_args:
            self.target = parsed_args['target']
        else:
            raise AngoraException(outlierCommand.EXCEPTION_03)

        self.parse_parameter = parsed_args['params']
        par_list = ['alg', 'tolerance', 'pct']

        if self.parse_parameter != None:
            for i in par_list:
                try:
                    if i in self.parse_parameter:
                        run = "self." +i + "= " + "self.parse_parameter" + "['"+ i +"']"
                        exec(run)
                except:
                    pass

    def distance(self, p1_x, p1_y, p2_x, p2_y):
        return math.sqrt((p2_y - p1_y)**2 + (p2_x - p1_x)**2)
    
    def find_neighbors(self, data, qx, qy, tolerance):
        Neighbors = []
        for idx, row in data.iterrows():
            d = self.distance(qx,qy,row['x'],row[self.target])
            if d <= tolerance :
                Neighbors.append(row)
        print 'finde',len(Neighbors)
        return Neighbors

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
        i = 0
    	data = df.toPandas()
        data['label'] = -1
        x = data[self.index].unique()
        tmp_result = []
        for val in x:
            tmp = data[data[self.index]==val]
            tmp['x'] = i
            i = i + 1
            tmp_result.append(tmp)
        data = pd.concat(tmp_result)
        
        data['label'] = data.apply(lambda row: self.dbscanAlgorithm(row.index,row['x'],row[self.target],row['label'],data),axis=1)
        print data
        return data

    def dbscanAlgorithm(self, idx, x, y, label, data):
        if label != -1: #undefined
            return label
        N = self.find_neighbors(data,x,y,self.tolerance)
        if len(N) < 30:
            label = 0 #noies
            return label 

        self.C = self.C + 1
        label = self.C
        self.S.extend(N)
        for r in self.S:
            print r.index
            if r['label'] == 0:
                data[data.index == r.index]['label'] = self.C
            if r['label'] != -1:
                return
            data[data.index == r.index]['label'] = self.C
            N = self.find_neighbors(data,r['x'],r[self.target],self.tolerance)
            if len(N) >= 30:
                self.S = list(set(self.S+N))

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

