from pyspark.sql import SparkSession
from pyspark.sql import SQLContext
import pickle
from pyspark.sql.types import StructType, StructField, IntegerType, StringType, BooleanType, DoubleType

spark = SparkSession.builder \
        .master("local") \
        .appName("Big data") \
        .getOrCreate()

sc = spark.sparkContext
sqlCtx = SQLContext(sc)

# dump
# --
#         import pickle
#         pickle.dump(result_basic, open('rect.data', 'wb'))

# load
# -- 
#        import pickle
#        data = pickle.load(open('rect.data'))

data = pickle.load(open("rect.data"))
st = []
for col in data.columns:
    if(data[col].dtype == 'float64'):
        st.append(StructField(col,DoubleType()))
    elif(data[col].dtype == 'bool'):
        st.append(StructField(col,BooleanType()))
    elif data[col].dtype == 'object':
        st.append(StructField(col,StringType()))
schema = StructType(st)
data = sqlCtx.createDataFrame(data, schema)
data.printSchema()
data.show()