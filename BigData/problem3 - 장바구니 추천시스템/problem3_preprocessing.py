#------------team1 sever
#			 166.104.229.161
#			 id : bigdata01
#			 pw : baekdu3116
#------------------------------

from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.sql import functions as F
from pyspark.sql.functions import struct
from pyspark.ml.fpm import FPGrowth



spark = SparkSession.builder \
    				.master("local") \
    				.appName("Example") \
				    .config("spark.some.config.option", "some-value") \
				    .getOrCreate()
sc = spark.sparkContext
sqlCtx = SQLContext(sc)

# df = spark.createDataFrame([
#     (0, [1, 2, 5]),
#     (1, [1, 2, 3, 5]),
#     (2, [1, 2])
# ], ["id", "items"])

# test_df = spark.createDataFrame([
# 	(0,[1]),
# 	(1,[2]),
# 	(2,[1,2]),
# 	(3,[3,5])
# ], ['id','items'])

# fpGrowth = FPGrowth(itemsCol="items", minSupport=0.5, minConfidence=0.6)
# model = fpGrowth.fit(df)

# # Display frequent itemsets.
# model.freqItemsets.show()

# # Display generated association rules.
# model.associationRules.show()

# # transform examines the input items against all the association rules and summarize the
# # consequents as prediction
# model.transform(test_df).show()
df = sqlCtx.read.format('csv') \
				.option('inferschema','true') \
				.load("train.csv")

test = sqlCtx.read.format('csv') \
				  .option('header','true') \
				  .load("test.csv")

test_rdd = test.rdd
test_rdd = test_rdd.map(lambda x: ([x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15],x[16],x[17],x[18],x[19],x[20],x[21],x[22],x[23],x[24],x[25],x[26],x[27],x[28],x[29],x[30],x[31],x[32],x[33],x[34],x[35],x[36]])).map(lambda x: [x])
test_df  = test_rdd.toDF(['items'])

df  = df.drop('_c2','_c3')
rdd = df.rdd.map(lambda x: (x[0],[str(x[1])])).reduceByKey(lambda x, y: x + y)
df  = rdd.toDF(['order_id','items'])
df  = df.sort("order_id", ascending=True)

fpGrowth = FPGrowth(itemsCol="items", minSupport=0.0001, minConfidence=0.0001)
model = fpGrowth.fit(df)
model.freqItemsets.show()
model.associationRules.show()

result = model.transform(test_df)
result.toPandas().to_csv('final.csv')
