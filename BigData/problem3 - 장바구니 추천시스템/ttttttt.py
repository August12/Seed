from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
from pyspark.sql import functions as F
from pyspark.sql.functions import struct
from pyspark.ml.fpm import FPGrowth
import pandas as pd

spark = SparkSession.builder \
    				.master("local") \
    				.appName("Example") \
				    .config("spark.some.config.option", "some-value") \
				    .getOrCreate()
sc = spark.sparkContext
sqlCtx = SQLContext(sc)

df = spark.createDataFrame([
    (0, [1, 2, 5]),
    (1, [1, 2, 4, 5]),
    (2, [1, 2, 3, 5]),
    (3, [1, 2]),
    (5, [1, 2, 3, 4, 5]),
    (0, [1, 2, 4]),
    (0, [1, 2, 3, 4])
], ["id", "items"])

df2 = spark.createDataFrame([
    (0, [1])
], ["id", "items"])

fpGrowth = FPGrowth(itemsCol="items", minSupport=0.1, minConfidence=0.3)
model = fpGrowth.fit(df)

# Display frequent itemsets.
t1 = model.freqItemsets
t1.show(t1.count())

# Display generated association rules.
t2 = model.associationRules
t2.sort(t2['confidence'].desc()).show(t2.count())

# transform examines the input items against all the association rules and summarize the
# consequents as prediction
t3 = model.transform(df2)
t3.show(t3.count())