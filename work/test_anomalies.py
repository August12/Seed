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
                ['20181029192000', 'mac', 20],
                ['20181029192010', 'mac', 25],
                ['20181029192020', 'mac', 30],
                ['20181029192030', 'mac', 35],
                ['20181029192040', 'mac', 40],
                ['20181029192050', 'mac', 45],
                ['20181029192100', 'mac', 50],
                ['20181029192110', 'mac', 55],
                ['20181029192120', 'mac', 60],
                ['20181029192130', 'mac', 65],
                ['20181029192140', 'mac', 70],
                ['20181029192150', 'mac', 75],
                ['20181029192200', 'mac', 80],
                ['20181029192210', 'mac', 85],
                ['20181029192220', 'mac', 90],
                ['20181029192230', 'mac', 95],
                ['20181029192240', 'mac', 300],
                ['20181029192250', 'mac', 105],
                ['20181029192300', 'mac', 110],
                ['20181029192310', 'mac2', 115],
                ['20181029192320', 'mac2', 120],
                ['20181029192330', 'mac2', 125],
                ['20181029192340', 'mac2', 130],
                ['20181029192350', 'mac2', 135],
                ['20181029192400', 'mac2', 40],
                ['20181029192410', 'mac2', 145],
                ['20181029192420', 'mac2', 150],
                ['20181029192430', 'mac2', 155],
                ['20181029192440', 'mac2', 160],
                ['20181029192450', 'mac2', 165]
                ]

        cls.test_df = anomaliesTests.sqlCtx.createDataFrame(data,['time','host','value'])

    def command_excute(self, cond):
        command = anomaliesCommand()
        parsed_args = command.parse(cond)
        df = command.execute(anomaliesTests.sqlCtx, self.test_df, parsed_args)
        df = df[(df['anomaly'] == 'true')]
        return df.collect()

    def test_case1(self):
        rows = []
        result = self.command_excute("time value")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:22:40', 300.0, 232.60939607918948, 27.390603920810506, True]]
        self.assertEqual(rows, answer)

    def test_case2(self):
        rows = []
        result = self.command_excute("time value alg=agile")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:22:40', 300.0, 95.0, 205.0, 138.57237123194113, -48.57237123194115, True],
                  [u'2018-10-29 19:22:50', 105.0, 300.0, 195.0, 176.57237123194113, -10.572371231941148, True]]
        self.assertEqual(rows, answer)

    def test_case3(self):
        rows = []
        result = self.command_excute("time value alg=agile bound=1")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:22:40', 300.0, 95.0, 205.0, 91.78618561597057, -1.7861856159705738, True],
                  [u'2018-10-29 19:22:50', 105.0, 300.0, 195.0, 129.78618561597057, 36.213814384029426, True],
                  [u'2018-10-29 19:24:00', 40.0, 135.0, 95.0, 69.78618561597057, -23.786185615970574, True],
                  [u'2018-10-29 19:24:10', 145.0, 40.0, 105.0, 89.78618561597057, -3.786185615970574, True]]
        self.assertEqual(rows, answer)

    def test_case4(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:22:40', 300.0, 1.8870128974151237, 1.517469442692368, 0.6097573007589749, True],
                  [u'2018-10-29 19:24:00', 40.0, 0.3281761560721954, 1.3884586494232134, 0.4807465074898201, True]]
        self.assertEqual(rows, answer)

    def test_case5(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust direct=below")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:24:00', 40.0, 0.3281761560721954, 1.3884586494232134, 0.4807465074898201, True]]
        self.assertEqual(rows, answer)

    def test_case6(self):
        rows = []
        result = self.command_excute("time value alert_window=last_60s")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = []
        self.assertEqual(rows, answer)

    def test_case7(self):
        rows = []
        result = self.command_excute("time value alg=agile alert_window=last_2m")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['predict'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:22:50', 105.0, 300.0, 195.0, 176.57237123194113, -10.572371231941148, True]]
        self.assertEqual(rows, answer)

    def test_case8(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust window=0.1 alert_window=last_1h")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        print rows
        answer = [[u'2018-10-29 19:22:40', 300.0, 1.8870128974151237, 1.517469442692368, 0.6097573007589749, True],
                  [u'2018-10-29 19:24:00', 40.0, 0.3281761560721954, 1.3884586494232134, 0.4807465074898201, True]]
        self.assertEqual(rows, answer)

    def test_case9(self):
        rows = []
        result = self.command_excute("time value by=host")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['host'],result[i]['upper'],result[i]['lower'],result[i]['anomaly']])
        answer = [[u'2018-10-29 19:22:40', 300, u'mac', 236.26848321180375, 23.73151678819623, True]]
        self.assertEqual(rows, answer)

    def test_case10(self):
        rows = []
        result = self.command_excute("time value alg=agile by=host bound=1")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['upper'],result[i]['lower'],result[i]['anomaly'],result[i]['host']])
        answer = [[u'2018-10-29 19:22:40', 300, 98.7521512703116, -8.752151270311607, True, u'mac'],
                  [u'2018-10-29 19:22:50', 105, 136.7521512703116, 29.247848729688393, True, u'mac'],
                  [u'2018-10-29 19:24:00', 40, 64.1295368550254, -18.1295368550254, True, u'mac2'],
                  [u'2018-10-29 19:24:10', 145, 84.1295368550254, 1.8704631449746003, True, u'mac2']]
        self.assertEqual(rows, answer)

    def test_case11(self):
        rows = []
        result = self.command_excute("time value bound=2 alg=robust window=0.1 alert_window=last_1h by=host")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['value'],result[i]['residuals'],result[i]['upper'],result[i]['lower'],result[i]['anomaly'],result[i]['host']])
        print rows
        answer = [[u'2018-10-29 19:22:40', 300, 1.6437781109445275, 1.3969409928870533, 0.6350018970863068, True, u'mac']]
        self.assertEqual(rows, answer)

if __name__ == "__main__":
    unittest.main()