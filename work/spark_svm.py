# coding=UTF-8
from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.ml.linalg import Vectors
from pyspark.ml.classification import LinearSVC
import pyspark.sql.functions as f

spark = SparkSession.builder \
        .master("local") \
        .appName("Big data") \
        .config("spark.some.config.option", "some-value") \
        .getOrCreate()

sc = spark.sparkContext
sqlCtx = SQLContext(sc)

def to_vector(features, df,Vectors):
    """선택한 필드들을 Vector 로 구성된 하나의 'features' 필드로 묶습니다."""

    tmp_df = df.select(features)
    rdd = tmp_df.rdd.map(lambda x: (Vectors.dense(list(x)),))
    vectored_df = rdd.toDF(['features'])
    return vectored_df

def to_double(features, df):
    """선택한 필드의 데이터 타입을 학습에 요구되는, double 형태로 변환합니다."""
    tmp_df = df.select(features)
    rdd = tmp_df.rdd.map(lambda x: [float(c) for c in x], )
    doubled_df = rdd.toDF(['label'])
    return doubled_df

def marge_dataframe(df1, df2, f, sign):
    """두 개의 데이터 프레임을 순서를 유지한채 병합하는 함수 입니다."""
    if sign:
        df1 = df1.withColumn('tmp', f.monotonically_increasing_id())
        df2 = df2.withColumn('tmp', f.monotonically_increasing_id())
        df1 = df1.join(df2, on=["tmp"])
        return df1
    else:
        df2 = df2.withColumn('tmp', f.monotonically_increasing_id())
        df2 = df2.join(df1, on=["tmp"]).drop("tmp")
        return df2

df = spark.read.format("csv").option("header", "true").load("pre_2015_05_28.csv")

features = df.columns
features.remove('fund')
features.remove('id')

df1 = to_vector(features, df, Vectors)
df1.show()
df2 = to_double('fund', df)
df2.show()
data = marge_dataframe(df1, df2, f, 1)
data.show()