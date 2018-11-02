import unittest
from pyspark import SparkContext, SQLContext
from outlier import outlierCommand


class outlierTests(unittest.TestCase):

    sqlCtx = None

    @classmethod
    def setUpClass(cls):
        """Do seting up class for test."""
        outlierTests.sc = SparkContext('local' )#spark://192.168.100.180:9090
        outlierTests.sqlCtx = SQLContext(outlierTests.sc)
        outlierTests.version = str(outlierTests.sqlCtx._sc.version)
        
        data = [
                ['20181029192000', 'host', 20], ['20181029192010', 'host', 25], ['20181029192020', 'host', 30],
                ['20181029192030', 'host', 35], ['20181029192040', 'host', 40], ['20181029192050', 'host', 45],
                ['20181029192100', 'host', 50], ['20181029192110', 'host', 55], ['20181029192120', 'host', 60],
                ['20181029192130', 'host', 65], ['20181029192140', 'host', 70], ['20181029192150', 'host', 75],
                ['20181029192200', 'host', 80], ['20181029192210', 'host', 85], ['20181029192220', 'host', 90],
                ['20181029192230', 'host', 95], ['20181029192240', 'host', 300], ['20181029192250', 'host', 105],
                ['20181029192300', 'host', 110], ['20181029192310', 'host', 115], ['20181029192320', 'host', 120],
                ['20181029192330', 'host', 125], ['20181029192340', 'host', 130], ['20181029192350', 'host', 135],
                ['20181029192400', 'host', 140], ['20181029192410', 'host', 145], ['20181029192420', 'host', 150],
                ['20181029192430', 'host', 155], ['20181029192440', 'host', 160], ['20181029192450', 'host', 165],

                ['20181029192000', 'guest', 0], ['20181029192010', 'guest', 5], ['20181029192020', 'guest', 10],
                ['20181029192030', 'guest', 15], ['20181029192040', 'guest', 20], ['20181029192050', 'guest', 25],
                ['20181029192100', 'guest', 30], ['20181029192110', 'guest', 35], ['20181029192120', 'guest', 40],
                ['20181029192130', 'guest', 45], ['20181029192140', 'guest', 50], ['20181029192150', 'guest', 55],
                ['20181029192200', 'guest', 60], ['20181029192210', 'guest', 65], ['20181029192220', 'guest', 70],
                ['20181029192230', 'guest', 75], ['20181029192240', 'guest', 80], ['20181029192250', 'guest', 85],
                ['20181029192300', 'guest', 90], ['20181029192310', 'guest', 95], ['20181029192320', 'guest', 100],
                ['20181029192330', 'guest', 105], ['20181029192340', 'guest', 110], ['20181029192350', 'guest', 115],
                ['20181029192400', 'guest', 120], ['20181029192410', 'guest', 125], ['20181029192420', 'guest', 130],
                ['20181029192430', 'guest', 135], ['20181029192440', 'guest', 140], ['20181029192450', 'guest', 145],

                ['20181029192000', 'guest2', 300], ['20181029192010', 'guest2', 305], ['20181029192020', 'guest2', 310],
                ['20181029192030', 'guest2', 315], ['20181029192040', 'guest2', 320], ['20181029192050', 'guest2', 325],
                ['20181029192100', 'guest2', 330], ['20181029192110', 'guest2', 335], ['20181029192120', 'guest2', 340],
                ['20181029192130', 'guest2', 345], ['20181029192140', 'guest2', 350], ['20181029192150', 'guest2', 355],
                ['20181029192200', 'guest2', 360], ['20181029192210', 'guest2', 365], ['20181029192220', 'guest2', 370],
                ['20181029192230', 'guest2', 375], ['20181029192240', 'guest2', 380], ['20181029192250', 'guest2', 385],
                ['20181029192300', 'guest2', 390], ['20181029192310', 'guest2', 395], ['20181029192320', 'guest2', 400],
                ['20181029192330', 'guest2', 405], ['20181029192340', 'guest2', 410], ['20181029192350', 'guest2', 415],
                ['20181029192400', 'guest2', 420], ['20181029192410', 'guest2', 425], ['20181029192420', 'guest2', 430],
                ['20181029192430', 'guest2', 435], ['20181029192440', 'guest2', 440], ['20181029192450', 'guest2', 445],
                ]

        cls.test_df = outlierTests.sqlCtx.createDataFrame(data,['time','value'])

    def command_excute(self, cond):
        command = anomaliesCommand()
        parsed_args = command.parse(cond)
        df = command.execute(anomaliesTests.sqlCtx, self.test_df, parsed_args)
        return df.collect()

    def test_case1(self):
        rows = []
        result = self.command_excute("target=host alg=dbscan tolerance=3")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['host'],result[i]['value'],result[i]['outlier']])
        answer = [[u'20181029192000', u'guest2', 300, True], [u'20181029192010', u'guest2', 305, True], [u'20181029192020', u'guest2', 310, True],
                [u'20181029192030', u'guest2', 315, True], [u'20181029192040', u'guest2', 320, True], [u'20181029192050', u'guest2', 325, True],
                [u'20181029192100', u'guest2', 330, True], [u'20181029192110', u'guest2', 335, True], [u'20181029192120', u'guest2', 340, True],
                [u'20181029192130', u'guest2', 345, True], [u'20181029192140', u'guest2', 350, True], [u'20181029192150', u'guest2', 355, True],
                [u'20181029192200', u'guest2', 360, True], [u'20181029192210', u'guest2', 365, True], [u'20181029192220', u'guest2', 370, True],
                [u'20181029192230', u'guest2', 375, True], [u'20181029192240', u'guest2', 380, True], [u'20181029192250', u'guest2', 385, True],
                [u'20181029192300', u'guest2', 390, True], [u'20181029192310', u'guest2', 395, True], [u'20181029192320', u'guest2', 400, True],
                [u'20181029192330', u'guest2', 405, True], [u'20181029192340', u'guest2', 410, True], [u'20181029192350', u'guest2', 415, True],
                [u'20181029192400', u'guest2', 420, True], [u'20181029192410', u'guest2', 425, True], [u'20181029192420', u'guest2', 430, True],
                [u'20181029192430', u'guest2', 435, True], [u'20181029192440', u'guest2', 440, True], [u'20181029192450', u'guest2', 445, True]]
        self.assertEqual(rows, answer)

    def test_case2(self):
        rows = []
        result = self.command_excute("host,guest,guest2 alg=mad tolerance=3 pct=0.1")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['host'],result[i]['value'],result[i]['outlier']])
        answer = [[u'20181029192000', u'guest2', 300, True], [u'20181029192010', u'guest2', 305, True], [u'20181029192020', u'guest2', 310, True],
                [u'20181029192030', u'guest2', 315, True], [u'20181029192040', u'guest2', 320, True], [u'20181029192050', u'guest2', 325, True],
                [u'20181029192100', u'guest2', 330, True], [u'20181029192110', u'guest2', 335, True], [u'20181029192120', u'guest2', 340, True],
                [u'20181029192130', u'guest2', 345, True], [u'20181029192140', u'guest2', 350, True], [u'20181029192150', u'guest2', 355, True],
                [u'20181029192200', u'guest2', 360, True], [u'20181029192210', u'guest2', 365, True], [u'20181029192220', u'guest2', 370, True],
                [u'20181029192230', u'guest2', 375, True], [u'20181029192240', u'guest2', 380, True], [u'20181029192250', u'guest2', 385, True],
                [u'20181029192300', u'guest2', 390, True], [u'20181029192310', u'guest2', 395, True], [u'20181029192320', u'guest2', 400, True],
                [u'20181029192330', u'guest2', 405, True], [u'20181029192340', u'guest2', 410, True], [u'20181029192350', u'guest2', 415, True],
                [u'20181029192400', u'guest2', 420, True], [u'20181029192410', u'guest2', 425, True], [u'20181029192420', u'guest2', 430, True],
                [u'20181029192430', u'guest2', 435, True], [u'20181029192440', u'guest2', 440, True], [u'20181029192450', u'guest2', 445, True]]
        self.assertEqual(rows, answer)




