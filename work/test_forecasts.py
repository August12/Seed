import unittest
from pyspark import SparkContext, SQLContext
from forecasts import forecastsCommand


class forecastsTests(unittest.TestCase):

    sqlCtx = None

    @classmethod
    def setUpClass(cls):
        """Do seting up class for test."""
        forecastsTests.sc = SparkContext('local' )#spark://192.168.100.180:9090
        forecastsTests.sqlCtx = SQLContext(forecastsTests.sc)
        forecastsTests.version = str(forecastsTests.sqlCtx._sc.version)
        
        data = [
                ['20181029192000', 'host', 20], ['20181029192010', 'host', 25], ['20181029192020', 'host', 30],
                ['20181029192030', 'host', 35], ['20181029192040', 'host', 40], ['20181029192050', 'host', 45],
                ['20181029192100', 'host', 50], ['20181029192110', 'host', 55], ['20181029192120', 'host', 60],
                ['20181029192130', 'host', 65], ['20181029192140', 'host', 70], ['20181029192150', 'host', 75],
                ['20181029192200', 'host', 80], ['20181029192210', 'host', 85], ['20181029192220', 'host', 90],
                ['20181029192230', 'host', 95], ['20181029192240', 'host', 100], ['20181029192250', 'host', 105],
                ['20181029192300', 'host', 110], ['20181029192310', 'host', 115], ['20181029192320', 'host', 120],
                ['20181029192330', 'host', 125], ['20181029192340', 'host', 130], ['20181029192350', 'host', 135],
                ['20181029192400', 'host', 140], ['20181029192410', 'host', 145], ['20181029192420', 'host', 150],
                ['20181029192430', 'host', 155], ['20181029192440', 'host', 160], ['20181029192450', 'host', 165],
                ]

        cls.test_df = forecastsTests.sqlCtx.createDataFrame(data,['time','host','value'])
        cls.test_df2 = forecastsTests.sqlCtx.read.format('csv') \
                                               .option("header", "true") \
                                               .option("inferschema", "true") \
                                               .load("mobigen_host.csv")

    def command_excute(self, cond):
        command = forecastsCommand()
        parsed_args = command.parse(cond)
        df = command.execute(forecastsTests.sqlCtx, self.test_df, parsed_args)
        return df.collect()

    def command_excute2(self, cond):
        command = forecastsCommand()
        parsed_args = command.parse(cond)
        df = command.execute(forecastsTests.sqlCtx, self.test_df2, parsed_args)
        return df.collect()

    def test_case1(self):
        rows = []
        result = self.command_excute("time value alg=seasonal deviations=1")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['host'],result[i]['value']])
        answer = []
        self.assertEqual(rows, answer)

    def test_case2(self):
        rows = []
        result = self.command_excute("time value alg=linear deviations=1")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['host'],result[i]['value']])
        answer = []
        self.assertEqual(rows, answer)