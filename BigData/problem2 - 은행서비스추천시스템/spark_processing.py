from pyspark.sql import SparkSession
from pyspark.sql import SQLContext

spark = SparkSession.builder \
        .master("local") \
        .appName("Big data") \
        .config("spark.driver.memory", "5g") \
        .config("spark.excutor.memory", "14") \
        .config("spark.excutor.cores", "5") \
        .getOrCreate()

sc = spark.sparkContext
sqlCtx = SQLContext(sc)

df = spark.read.format("csv").option("header", "true").load("2015-05-28.csv")
df = df.drop('id').drop('sp_index')

freq = df.stat.freqItems(df.columns,0.5)
print freq.collect()[0]