# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.ml.linalg import Vectors
import pandas as pd
import numpy as np
from pyspark.ml.feature import StringIndexer, VectorAssembler, OneHotEncoder
from pyspark.ml import Pipeline
from pyspark.ml.classification import LogisticRegression
from pyspark.ml.evaluation import BinaryClassificationEvaluator

filename = '2015-05-28.csv'

MEAN 	 = 1
MEDIAN 	 = 2
FREQUENT = 3

def nanToValue(df,target,method):
	if method == 3:
		item_counts = df[target].value_counts()
		result = item_counts.index[0]


	df[target] = df[target].replace(np.NaN,result)
	return df

def preCate(df):
	tmp = []
	for value in df['cate']:
		if(value == '01 - TOP'):
			tmp.append(1)
		elif(value == '02 - PARTICULARES'):
			tmp.append(2)
		else:
			tmp.append(3)
	df['cate'] = tmp
	return df

spark = SparkSession.builder \
        .master("local") \
        .appName("Big data") \
        .config("spark.driver.memory", "5g") \
        .config("spark.excutor.memory", "14") \
        .config("spark.excutor.cores", "5") \
        .getOrCreate()

sc = spark.sparkContext
sqlCtx = SQLContext(sc)

df = spark.read.csv(filename,header=True,inferSchema=True)
df = df.drop('sp_index')

data = df.toPandas()
for col in data.columns:
	data = nanToValue(data,col,3)

# fund_possible_data = data[data['fund']==1]
# fund_not_data = data[data['fund']==0]
# pd.set_option('display.max_rows', 200)  # or 1000
# print fund_not_data.drop_duplicates().channel.value_counts()
# fund_not_data = fund_not_data.sample(20000,random_state=99)

# train = pd.concat([fund_possible_data,fund_not_data])
# train.to_csv('pre_5.csv',header=True,index=False)
data.to_csv('pre_test.csv',header=True,index=False)