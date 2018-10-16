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
                ['2012-11-1', 20],
                ['2013-11-2', 25],
                ['2014-11-3', 30],
                ['2015-11-4', 35],
                ['2011-11-5', 40],
                ['2011-11-6', 45],
                ['2011-11-7', 50],
                ['2011-11-8', 55],
                ['2011-11-9', 60],
                ['2011-11-10', 65],
                ['2011-11-11', 70],
                ['2011-11-12', 75],
                ['2011-11-13', 80],
                ['2011-11-14', 85],
                ['2011-11-15', 90],
                ['2011-11-16', 95],
                ['2011-11-17', 300],
                ['2011-11-18', 105],
                ['2011-11-19', 110],
                ['2011-11-20', 115],
                ['2011-11-21', 120],
                ['2011-11-22', 125],
                ['2011-11-23', 130],
                ['2011-11-24', 135],
                ['2011-11-25', 140],
                ['2011-11-26', 145],
                ['2011-11-27', 150],
                ['2011-11-28', 155],
                ['2012-11-29', 260],
                ['2013-11-30', 265]
                ]

        cls.test_df = anomaliesTests.sqlCtx.createDataFrame(data,['time','value'])

    def command_excute(self, cond):
        command = anomaliesCommand()
        parsed_args = command.parse(cond)
        df = command.execute(anomaliesTests.sqlCtx, self.test_df, parsed_args)
        df = df[(df['anomaly'] == 'true')]
        return df.collect()

    def test_case1(self):
        """Do test Normal case."""
        rows = []
        result = self.command_excute("time value")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 252.60696367583353, 7.393036324166459, True]]
        self.assertEqual(rows, answer)

    def test_case2(self):
        rows = []
        result = self.command_excute("time value alg=agile")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 92.44817731929673, 207.55182268070325, 136.25077621585882, -41.684356681968765, True],
                  [u'2011-11-18', 105.0, 291.9416125872528, 186.9416125872528, 172.23617936267203, -5.698953535155567, True]]
        self.assertEqual(rows, answer)

    def test_case3(self):
        rows = []
        result = self.command_excute("time value bound=2")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 252.60696367583353, 7.393036324166459, True]]
        self.assertEqual(rows, answer)

    def test_case4(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 1.700276953051652, 1.3584429311504276, 0.6959527347961093, True]]
        self.assertEqual(rows, answer)

    def test_case5(self):
        rows = []
        result = self.command_excute("time value alg=agile bound=4")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = []
        self.assertEqual(rows, answer)


if __name__ == "__main__":
    unittest.main()