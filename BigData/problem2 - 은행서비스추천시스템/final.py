# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.ml.linalg import Vectors
import pandas as pd
import numpy as np
from pyspark.ml.feature import StringIndexer, VectorAssembler, OneHotEncoder
from pyspark.ml import Pipeline
from pyspark.ml.classification import LogisticRegression
from pyspark.ml.evaluation import BinaryClassificationEvaluator, MulticlassClassificationEvaluator
from pyspark.ml.classification import RandomForestClassifier
from pyspark.ml.tuning import CrossValidator, ParamGridBuilder

filename = 'pre_test.csv'

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
test = spark.read.csv(filename,header=True,inferSchema=True)

# numeric_features = [t[0] for t in df.dtypes if t[1] == 'double']
# print df.select(numeric_features).describe().toPandas().transpose()
# numeric_data = df.select(numeric_features).toPandas()

# axs = pd.scatter_matrix(numeric_data, figsize=(5, 5));
# n = len(numeric_data.columns)
# for i in range(n):
#     v = axs[i, 0]
#     v.yaxis.label.set_rotation(0)
#     v.yaxis.label.set_ha('right')
#     v.set_yticks(())
#     h = axs[n-1, i]
#     h.xaxis.label.set_rotation(90)
#     h.set_xticks(())

# for col in df.columns:
# 	df = nanToValue(df,col,3)

labelColumns = ['fund', 'eaccount', 'loans', 'credit_card']
categorigalColumns = ['employee','gender','res_index',
				  'for_idx','channel','res_code','cate']
stages = []

for categoricalCol in categorigalColumns:
	stringIndexer = StringIndexer(inputCol=categoricalCol,outputCol=categoricalCol+'Index')
	encoder = OneHotEncoder(inputCol=stringIndexer.getOutputCol(),outputCol=categoricalCol+"classVec")
	stages += [stringIndexer, encoder]

numericColumns = df.columns
for col in categorigalColumns:
	numericColumns.remove(col)

for col in labelColumns:
	numericColumns.remove(col)
numericColumns.remove('id')

assemblerInputs = [c+'classVec' for c in categorigalColumns] + numericColumns

assembler = VectorAssembler(inputCols=assemblerInputs, outputCol='features')
stages += [assembler]

pipeline = Pipeline(stages = stages)
pipelineModel = pipeline.fit(df)
df = pipelineModel.transform(df)
df.printSchema()

df.select('employeeclassVec','genderclassVec','res_indexclassVec','for_idxclassVec','channelclassVec','res_codeclassVec','cateclassVec').show()
df.select(numericColumns).show()
df.select('features').show()

df_fund_true = df.filter(df['fund']==1)
df_fund_false = df.filter(df['fund']==0)
df_fund_false = df_fund_false.sample(False,0.05,seed=2018)
df_fund_false.show()
train = df_fund_true.union(df_fund_false)

pipeline = Pipeline(stages = stages)
pipelineModel = pipeline.fit(test)
test = pipelineModel.transform(test)
test.printSchema()

# train, test = df.randomSplit([0.7, 0.3], seed=2018)
print 'train data count: ', train.count()
print 'test data count: ', test.count()

lr = LogisticRegression(featuresCol = 'features', labelCol = 'fund', maxIter=100,
						regParam=0.1)

rf = RandomForestClassifier(labelCol="fund", featuresCol="features", numTrees=100)


evaluator = MulticlassClassificationEvaluator(labelCol='fund', predictionCol="prediction", metricName="weightedPrecision")
grid = ParamGridBuilder().addGrid(rf.numTrees, [10]).build()

cv = CrossValidator(estimator=rf, estimatorParamMaps=grid, evaluator=evaluator, numFolds=10)
cvModel = cv.fit(train)
predictions = cvModel.transform(test)
predictions.select('id','fund','prediction').filter(predictions['prediction']==1).show(predictions.count())
print predictions.count()
print('Test precison', evaluator.evaluate(predictions))

# rf = RandomForestClassifier(labelCol="fund", featuresCol="features", numTrees=10)
# rfModel = rf.fit(train)

# predictions = rfModel.transform(test)
# predictions.select('id','fund','prediction').show(100)

# evaluator = MulticlassClassificationEvaluator(
#     labelCol="fund", predictionCol="prediction", metricName="weightedPrecision")
# print('Test precison', evaluator.evaluate(predictions))













