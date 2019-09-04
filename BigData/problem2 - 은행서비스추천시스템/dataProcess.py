from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.ml.feature import StringIndexer
import pandas as pd
import numpy as np

def employeePreprocessing(df):
	employee = []

	for value in df['employee']:
		if(value == 'A'):
			employee.append([1,0,0,0,0])
		elif(value == 'B'):
			employee.append([0,1,0,0,0])
		elif(value == 'F'):
			employee.append([0,0,1,0,0])
		elif(value == 'N'):
			employee.append([0,0,0,1,0])
		elif(value == 'P'):
			employee.append([0,0,0,0,1])
		else:
			employee.append([0,0,0,0,0])

	return employee

def genderPreprocessing(df):
	gender = []

	for value in df['gender']:
		if(value == 'V'):
			gender.append([1,0])
		elif(value == 'H'):
			gender.append([0,1])
		else:
			gender.append([0,0])

	df = df.drop(columns=['gender'])
	df.insert(2,"gender",gender)

	return df

def res_indexPreprocessing(df):
	tmp = []

	for value in df['res_index']:
		if(value == 'S'):
			tmp.append([1,0])
		elif(value == 'N'):
			tmp.append([0,1])
		else:
			tmp.append([0,0])

	df = df.drop(columns=['res_index'])
	df.insert(3,"res_index",tmp)

	return df

def for_idxPreprocessing(df):
	tmp = []

	for value in df['for_idx']:
		if(value == 'S'):
			tmp.append([1,0])
		elif(value == 'N'):
			tmp.append([0,1])
		else:
			tmp.append([0,0])

	df = df.drop(columns=['for_idx'])
	df.insert(4,"for_idx",tmp)

	return df

def sp_indexPreprocessing(df):
	tmp = []

	for value in df['sp_index']:
		if(value == 'S'):
			tmp.append([1,0])
		elif(value == 'N'):
			tmp.append([0,1])
		else:
			tmp.append([0,0])

	df = df.drop(columns=['sp_index'])
	df.insert(5,"sp_index",tmp)

	return df

def catePreprocessing(df):
	tmp = []

	for value in df['cate']:
		if(value == '01 - TOP'):
			tmp.append(1)
		elif(value == '02 - PARTICULARES'):
			tmp.append(2)
		else:
			tmp.append(3)

	df = df.drop(columns=['cate'])
	df.insert(13,"cate",tmp)

	return df

def channelPreprocessing(df):

	df = spark.createDataFrame(df)
	indexer = StringIndexer(inputCol='channel',outputCol='channel')
	indexed = indexer.fit(df).transform(df)
	return indexed

def dataPreprocessing(df):
	df = employeePreprocessing(df)
	df = genderPreprocessing(df)
	df = res_indexPreprocessing(df)
	df = for_idxPreprocessing(df)
	df = sp_indexPreprocessing(df)
	df = catePreprocessing(df)

	return df


spark = SparkSession.builder \
	    .master("local") \
	    .appName("processing") \
	    .config("spark.some.config.option", "some-value") \
	    .getOrCreate()

# df = pd.read_csv('2015-01-28.csv',index_col=False)
# preprocessed = dataPreprocessing(df)
# print channelPreprocessing(preprocessed)

df = spark.read \
		.format("csv") \
		.option("header", "true") \
		.load("2015-01-28.csv")

df.show()
print df.select('employee').collect() #employeePreprocessing(df)
