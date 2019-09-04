# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.ml.linalg import Vectors
from pyspark.ml.classification import LinearSVC
from pyspark.ml.classification import LogisticRegression

spark = SparkSession.builder \
        .master("local") \
        .appName("Big data") \
        .config("spark.driver.memory", "5g") \
        .config("spark.excutor.memory", "14") \
        .config("spark.excutor.cores", "5") \
        .getOrCreate()

sc = spark.sparkContext
sqlCtx = SQLContext(sc)

def to_vector(features, df,Vectors):
    """선택한 필드들을 Vector 로 구성된 하나의 'features' 필드로 묶습니다."""

    tmp_df = df.select(features)
    rdd = tmp_df.rdd.flatMap(lambda x: Vectors.dense(x),)
    vectored_df = rdd.toDF(['features'])
    return vectored_df

def to_double(features, df):
    """선택한 필드의 데이터 타입을 학습에 요구되는, double 형태로 변환합니다."""
    tmp_df = df.select(features)
    rdd = tmp_df.rdd.map(lambda x: [int(c) for c in x], )
    doubled_df = rdd.toDF(['label'])
    return doubled_df

def marge_df(f_df, l_df):
    f_rdd  = f_df.rdd
    l_rdd  = l_df.rdd
    m_rdd  = f_rdd.union(l_rdd)
    m_rdd  = m_rdd.flatMap(lambda x: list(x))
    m_rdd  = m_rdd.map(lambda x: (Vectors.dense(x[:len(x)-2]),int(x[len(x)-1])))
    result = m_rdd.toDF(['features','label'])
    return result

df = spark.read.format("csv").option("header", "true").load("pre_2015_05_28.csv")

features = df.columns
features.remove('fund')
features.remove('eaccount')
features.remove('loans')
features.remove('credit_card')
print features

lr = LogisticRegression(featuresCol=features, labelCol='fund', maxIter=100, regParam=0.3, elasticNetParam=0.8)
lrModel = lr.fit(df)

print("Coefficients: " + str(lrModel.coefficients))
print("Intercept: " + str(lrModel.intercept))


