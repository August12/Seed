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
                ['2011-11-1', 20],
                ['2011-11-2', 25],
                ['2011-11-3', 30],
                ['2011-11-4', 35],
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
                ['2011-11-29', 160],
                ['2011-11-30', 165]
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
        answer = [[u'2011-11-17', 300.0, 231.83187734943292, 28.168122650567085, True]]
        self.assertEqual(rows, answer)

    def test_case2(self):
            rows = []
            result = self.command_excute("time value bound=2")
            for i in range(len(result)) :
                rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
            answer = [[u'2011-11-17', 300.0, 231.83187734943292, 28.168122650567085, True]]
            self.assertEqual(rows, answer)

    def test_case3(self):
        rows = []
        result = self.command_excute("time value alg=agile")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 87.9980845315313, 212.0019154684687, 132.29558254406456, -29.765839074173236, True],
                  [u'2011-11-18', 105.0, 277.8886879943094, 172.8886879943094, 164.7677545426419, 2.7063329244041228, True]]
        self.assertEqual(rows, answer)

    def test_case4(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 1.716913377851342, 1.3619467026789147, 0.7003013230003994, True]]
        self.assertEqual(rows, answer)

    def test_case5(self):
        rows = []
        result = self.command_excute("time value alg=agile bound=4")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = []
        self.assertEqual(rows, answer)

    def test_case6(self):
        rows = []
        result = self.command_excute("time value alg=agile bound=1")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 87.9980845315313, 212.0019154684687, 91.78022713950512, 10.749516330386214, True],
                  [u'2011-11-18', 105.0, 277.8886879943094, 172.8886879943094, 124.25239913808247, 43.22168832896357, True],
                  [u'2011-11-19', 110.0, 97.26104079800828, 12.738959201991719, 124.62092100484398, 43.590210195725085, True],
                  [u'2011-11-20', 115.0, 101.89251893124677, 13.10748106875323, 124.98944287160549, 43.9587320624866, True],
                  [u'2011-11-21', 120.0, 106.52399706448526, 13.476002935514742, 125.357964738367, 44.32725392924811, True]]
        self.assertEqual(rows, answer)

    def test_case7(self):
        rows = []
        result = self.command_excute("time value window=10")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = []
        self.assertEqual(rows, answer)

    def test_case8(self):
        rows = []
        result = self.command_excute("time value window=20")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 231.83187734943292, 28.168122650567085, True]]
        self.assertEqual(rows, answer)

    def test_case9(self):
        rows = []
        result = self.command_excute("time value alg=agile window=20")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 87.9980845315313, 212.0019154684687, 132.29558254406456, -29.765839074173236, True],
                  [u'2011-11-18', 105.0, 277.8886879943094, 172.8886879943094, 164.7677545426419, 2.7063329244041228, True]]
        self.assertEqual(rows, answer)

    def test_case10(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust direct=below")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = []
        self.assertEqual(rows, answer)

    def test_case11(self):
        """Do test Normal case."""
        rows = []
        result = self.command_excute("time value window=0.5")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2011-11-17', 300.0, 231.83187734943292, 28.168122650567085, True]]
        self.assertEqual(rows, answer)

if __name__ == "__main__":
    unittest.main()