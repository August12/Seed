from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.ml.recommendation import ALS
from pyspark.sql import Row

spark = SparkSession.builder \
    				.master("local") \
    				.appName("Example") \
				    .config("spark.some.config.option", "some-value") \
				    .getOrCreate()
sc = spark.sparkContext
sqlCtx = SQLContext(sc)

lines = spark.read.text("sample_movielens_ratings.txt").rdd
parts = lines.map(lambda row: row.value.split("::"))
ratingsRDD = parts.map(lambda p: Row(userId=int(p[0]), movieId=int(p[1]),
                                     rating=float(p[2]), timestamp=long(p[3])))
ratings = spark.createDataFrame(ratingsRDD)
(training, test) = ratings.randomSplit([0.8, 0.2])

training.show()
training.printSchema()

als = ALS(maxIter=5, regParam=0.01, userCol="userId", itemCol="movieId", ratingCol="rating",
          coldStartStrategy="drop")
model = als.fit(training)

# Evaluate the model by computing the RMSE on the test data
# predictions = model.transform(test)
# predictions.show(predictions.count())

userRecs = model.recommendForAllUsers(10)

userRecs.show()

movieRecs = model.recommendForAllItems(10)

movieRecs.show()

# df = sqlCtx.read.format('csv') \
# 				.option('header', 'true') \
# 				.option('inferschema','true') \
# 				.load("order_products__prior.csv")

# (training, test) = df.randomSplit([0.8, 0.2])
# als = ALS(maxIter=5, regParam=0.01, userCol="order_id", itemCol="product_id", ratingCol="add_to_cart_order",
#           coldStartStrategy="drop")
# model = als.fit(training)

# movieRecs = model.recommendForAllItems(10)

