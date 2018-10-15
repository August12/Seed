import unittest
from pyspark import SparkContext, SQLContext
from anomalies import anomaliesCommand


class anomaliesTests(unittest.TestCase):

    sqlCtx = None

    @classmethod
    def setUpClass(cls):
        """Do seting up class for test."""
        anomaliesTests.sc = SparkContext('local' )#spark://192.168.100.180:9090
        anomaliesTests.sqlCtx = SQLContext(anomaliesTests.sc)
        anomaliesTests.version = str(anomaliesTests.sqlCtx._sc.version)
        
        data = [
                [2011-11-1, 20],
                [2011-11-2, 25],
                [2011-11-3, 30],
                [2011-11-4, 35],
                [2011-11-5, 40],
                [2011-11-6, 45],
                [2011-11-7, 50],
                [2011-11-8, 55],
                [2011-11-9, 60],
                [2011-11-10, 65],
                [2011-11-11, 70],
                [2011-11-12, 75],
                [2011-11-13, 80],
                [2011-11-14, 85],
                [2011-11-15, 90],
                [2011-11-16, 95],
                [2011-11-17, 100],
                [2011-11-18, 105],
                [2011-11-19, 110],
                [2011-11-20, 115],
                [2011-11-21, 120],
                [2011-11-22, 125],
                [2011-11-23, 130],
                [2011-11-24, 135],
                [2011-11-25, 140],
                [2011-11-26, 145],
                [2011-11-27, 150],
                [2011-11-28, 155]]

        cls.test_df = anomaliesTests.sqlCtx.createDataFrame(data,["time","value"])

    def command_excute(self, cond):
        command = anomaliesCommand()
        parsed_args = command.parse(cond)
        df = command.execute(anomaliesTests.sqlCtx, self.test_df, parsed_args)
        return df

    def test_case1(self):
        """Do test Normal case."""
        print self.command_excute("time value")
        # v = []
        # self.assertEqual(ret, v)

    def test_case2(self):
        print self.command_excute("time value alg=basic")

    def test_case3(self):
        print self.command_excute("time value bound=2")

    def test_case4(self):
        print self.command_excute("time value bound=2 alg=robust") 

    def test_case5(self):
        print self.command_excute("time value bound=2")

    def test_case6(self):
        print self.command_excute("time value alg=agile bound=4")


if __name__ == "__main__":
    unittest.main()