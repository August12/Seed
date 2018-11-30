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
                ['20181029192230', 'host', 95], ['20181029192240', 'host', 100], ['20181029192250', 'host', 105],
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

        cls.test_df = outlierTests.sqlCtx.createDataFrame(data,['time','host','value'])
        cls.test_df2 = outlierTests.sqlCtx.read.format('csv') \
                                               .option("header", "true") \
                                               .option("inferschema", "true") \
                                               .load("DATETIME_HOST_CNT.csv")

    def command_excute(self, cond):
        command = outlierCommand()
        parsed_args = command.parse(cond)
        df = command.execute(outlierTests.sqlCtx, self.test_df, parsed_args)
        df = df.filter(df['outlier']==True)
        return df.collect()

    def command_excute2(self, cond):
        command = outlierCommand()
        parsed_args = command.parse(cond)
        df = command.execute(outlierTests.sqlCtx, self.test_df2, parsed_args)
        df = df.filter(df['outlier']==True)
        return df.collect()

    def test_case1(self):
        rows = []
        result = self.command_excute("value alg=dbscan by=host tolerance=0.3")
        for i in range(len(result)) :
            rows.append([result[i]['time'],result[i]['host'],result[i]['value'],result[i]['outlier']])
        answer = [[u'20181029192000', u'guest2', u'300', u'True'], [u'20181029192010', u'guest2', u'305', u'True'],
                  [u'20181029192020', u'guest2', u'310', u'True'], [u'20181029192030', u'guest2', u'315', u'True'],
                  [u'20181029192040', u'guest2', u'320', u'True'], [u'20181029192050', u'guest2', u'325', u'True'],
                  [u'20181029192100', u'guest2', u'330', u'True'], [u'20181029192110', u'guest2', u'335', u'True'],
                  [u'20181029192120', u'guest2', u'340', u'True'], [u'20181029192130', u'guest2', u'345', u'True'],
                  [u'20181029192140', u'guest2', u'350', u'True'], [u'20181029192150', u'guest2', u'355', u'True'],
                  [u'20181029192200', u'guest2', u'360', u'True'], [u'20181029192210', u'guest2', u'365', u'True'],
                  [u'20181029192220', u'guest2', u'370', u'True'], [u'20181029192230', u'guest2', u'375', u'True'],
                  [u'20181029192240', u'guest2', u'380', u'True'], [u'20181029192250', u'guest2', u'385', u'True'],
                  [u'20181029192300', u'guest2', u'390', u'True'], [u'20181029192310', u'guest2', u'395', u'True'],
                  [u'20181029192320', u'guest2', u'400', u'True'], [u'20181029192330', u'guest2', u'405', u'True'],
                  [u'20181029192340', u'guest2', u'410', u'True'], [u'20181029192350', u'guest2', u'415', u'True'],
                  [u'20181029192400', u'guest2', u'420', u'True'], [u'20181029192410', u'guest2', u'425', u'True'],
                  [u'20181029192420', u'guest2', u'430', u'True'], [u'20181029192430', u'guest2', u'435', u'True'],
                  [u'20181029192440', u'guest2', u'440', u'True'], [u'20181029192450', u'guest2', u'445', u'True']]
        self.assertEqual(rows, answer)

    def test_case2(self):
        rows = []
        result = self.command_excute("value alg=mad by=host tolerance=1 pct=0.1")
        for i in range(len(result)) :
            rows.append([result[i]['DATETIME_10M'],result[i]['time'],result[i]['host'],result[i]['value'],result[i]['outlier']])
        answer = []
        self.assertEqual(rows, answer)

    def test_case3(self):
        rows = []
        result = self.command_excute2("CNT alg=dbscan by=HOST tolerance=0.3")
        for i in range(len(result)) :
            rows.append([result[i]['DATETIME_10M'],result[i]['CNT'],result[i]['HOST'],result[i]['outlier']])
        answer = [[u'20181103043000', u'4292', u'gcs1', u'True'], [u'20181103025000', u'4296', u'gcs1', u'True'],
                  [u'20181102124000', u'4292', u'gcs1', u'True'], [u'20181102094000', u'4315', u'gcs1', u'True'],
                  [u'20181102220000', u'4366', u'gcs1', u'True'], [u'20181102131000', u'4326', u'gcs1', u'True'],
                  [u'20181102175000', u'4386', u'gcs1', u'True'], [u'20181103053000', u'4325', u'gcs1', u'True'],
                  [u'20181102115000', u'4292', u'gcs1', u'True'], [u'20181102183000', u'4335', u'gcs1', u'True'],
                  [u'20181102171000', u'4295', u'gcs1', u'True'], [u'20181103014000', u'4292', u'gcs1', u'True'],
                  [u'20181102184000', u'4292', u'gcs1', u'True'], [u'20181102140000', u'4399', u'gcs1', u'True'],
                  [u'20181102190000', u'4399', u'gcs1', u'True'], [u'20181102125000', u'4325', u'gcs1', u'True'],
                  [u'20181102185000', u'4296', u'gcs1', u'True'], [u'20181103024000', u'4325', u'gcs1', u'True'],
                  [u'20181103013000', u'4325', u'gcs1', u'True'], [u'20181102173000', u'4292', u'gcs1', u'True'],
                  [u'20181103032000', u'4292', u'gcs1', u'True'], [u'20181102192000', u'4325', u'gcs1', u'True'],
                  [u'20181102113000', u'4325', u'gcs1', u'True'], [u'20181103005000', u'4296', u'gcs1', u'True'],
                  [u'20181102143000', u'4325', u'gcs1', u'True'], [u'20181103034000', u'4292', u'gcs1', u'True'],
                  [u'20181102195000', u'4339', u'gcs1', u'True'], [u'20181103082000', u'4325', u'gcs1', u'True'],
                  [u'20181102155000', u'4292', u'gcs1', u'True'], [u'20181102135000', u'4292', u'gcs1', u'True'],
                  [u'20181103002000', u'4292', u'gcs1', u'True'], [u'20181103051000', u'4326', u'gcs1', u'True'],
                  [u'20181102235000', u'4339', u'gcs1', u'True'], [u'20181102222000', u'4292', u'gcs1', u'True'],
                  [u'20181103064000', u'4325', u'gcs1', u'True'], [u'20181102130000', u'4366', u'gcs1', u'True'],
                  [u'20181102122000', u'4325', u'gcs1', u'True'], [u'20181103093000', u'4292', u'gcs1', u'True'],
                  [u'20181103000000', u'4366', u'gcs1', u'True'], [u'20181102151000', u'4344', u'gcs1', u'True'],
                  [u'20181103050000', u'4366', u'gcs1', u'True'], [u'20181103020000', u'4366', u'gcs1', u'True'],
                  [u'20181102202000', u'4292', u'gcs1', u'True'], [u'20181103060000', u'4399', u'gcs1', u'True'],
                  [u'20181102090000', u'4383', u'gcs1', u'True'], [u'20181103001000', u'4324', u'gcs1', u'True'],
                  [u'20181103071000', u'4326', u'gcs1', u'True'], [u'20181102182000', u'4292', u'gcs1', u'True'],
                  [u'20181102191000', u'4298', u'gcs1', u'True'], [u'20181102234000', u'4292', u'gcs1', u'True'],
                  [u'20181102093000', u'4325', u'gcs1', u'True'], [u'20181102080000', u'4070', u'gcs1', u'True'],
                  [u'20181103042000', u'4325', u'gcs1', u'True'], [u'20181102223000', u'4292', u'gcs1', u'True'],
                  [u'20181102200000', u'4376', u'gcs1', u'True'], [u'20181102083000', u'4292', u'gcs1', u'True'],
                  [u'20181103065000', u'4294', u'gcs1', u'True'], [u'20181102153000', u'4327', u'gcs1', u'True'],
                  [u'20181102231000', u'4293', u'gcs1', u'True'], [u'20181102091000', u'4406', u'gcs1', u'True'],
                  [u'20181102111000', u'4326', u'gcs1', u'True'], [u'20181102203000', u'4292', u'gcs1', u'True'],
                  [u'20181103091000', u'4326', u'gcs1', u'True'], [u'20181103085000', u'4325', u'gcs1', u'True'],
                  [u'20181103021000', u'4293', u'gcs1', u'True'], [u'20181102213000', u'4320', u'gcs1', u'True'],
                  [u'20181102114000', u'4292', u'gcs1', u'True'], [u'20181102225000', u'4296', u'gcs1', u'True'],
                  [u'20181102152000', u'4360', u'gcs1', u'True'], [u'20181103030000', u'4366', u'gcs1', u'True'],
                  [u'20181103090000', u'4366', u'gcs1', u'True'], [u'20181102172000', u'4325', u'gcs1', u'True'],
                  [u'20181103041000', u'4293', u'gcs1', u'True'], [u'20181102205000', u'4296', u'gcs1', u'True'],
                  [u'20181103062000', u'4325', u'gcs1', u'True'], [u'20181103035000', u'4296', u'gcs1', u'True'],
                  [u'20181103040000', u'4397', u'gcs1', u'True'], [u'20181102180000', u'4394', u'gcs1', u'True'],
                  [u'20181103063000', u'4290', u'gcs1', u'True'], [u'20181102133000', u'4296', u'gcs1', u'True'],
                  [u'20181102230000', u'4399', u'gcs1', u'True'], [u'20181102095000', u'4392', u'gcs1', u'True'],
                  [u'20181103054000', u'4292', u'gcs1', u'True'], [u'20181102134000', u'4325', u'gcs1', u'True'],
                  [u'20181102163000', u'4325', u'gcs1', u'True'], [u'20181102102000', u'4335', u'gcs1', u'True'],
                  [u'20181102201000', u'4326', u'gcs1', u'True'], [u'20181103073000', u'4325', u'gcs1', u'True'],
                  [u'20181102081000', u'4293', u'gcs1', u'True'], [u'20181103083000', u'4292', u'gcs1', u'True'],
                  [u'20181102105000', u'4325', u'gcs1', u'True'], [u'20181103084000', u'4292', u'gcs1', u'True'],
                  [u'20181102165000', u'4325', u'gcs1', u'True'], [u'20181103003000', u'4292', u'gcs1', u'True'],
                  [u'20181102110000', u'4366', u'gcs1', u'True'], [u'20181103023000', u'4292', u'gcs1', u'True'],
                  [u'20181102132000', u'4292', u'gcs1', u'True'], [u'20181103074000', u'4292', u'gcs1', u'True'],
                  [u'20181102210000', u'4399', u'gcs1', u'True'], [u'20181102085000', u'4323', u'gcs1', u'True'],
                  [u'20181103010000', u'4389', u'gcs1', u'True'], [u'20181103012000', u'4292', u'gcs1', u'True'],
                  [u'20181103011000', u'4303', u'gcs1', u'True'], [u'20181102224000', u'4325', u'gcs1', u'True'],
                  [u'20181102104000', u'4292', u'gcs1', u'True'], [u'20181103031000', u'4326', u'gcs1', u'True'],
                  [u'20181102162000', u'4292', u'gcs1', u'True'], [u'20181102103000', u'4315', u'gcs1', u'True'],
                  [u'20181102215000', u'4329', u'gcs1', u'True'], [u'20181102174000', u'4325', u'gcs1', u'True'],
                  [u'20181102161000', u'4293', u'gcs1', u'True'], [u'20181102150000', u'4366', u'gcs1', u'True'],
                  [u'20181102181000', u'4358', u'gcs1', u'True'], [u'20181103033000', u'4325', u'gcs1', u'True'],
                  [u'20181102141000', u'4293', u'gcs1', u'True'], [u'20181103055000', u'4296', u'gcs1', u'True'],
                  [u'20181103075000', u'4292', u'gcs1', u'True'], [u'20181102121000', u'4293', u'gcs1', u'True'],
                  [u'20181102160000', u'4399', u'gcs1', u'True'], [u'20181102164000', u'4292', u'gcs1', u'True'],
                  [u'20181103092000', u'4292', u'gcs1', u'True'], [u'20181102232000', u'4292', u'gcs1', u'True'],
                  [u'20181103081000', u'4293', u'gcs1', u'True'], [u'20181102212000', u'4297', u'gcs1', u'True'],
                  [u'20181102214000', u'4292', u'gcs1', u'True'], [u'20181102082000', u'4325', u'gcs1', u'True'],
                  [u'20181103045000', u'4296', u'gcs1', u'True'], [u'20181102100000', u'4423', u'gcs1', u'True'],
                  [u'20181102194000', u'4292', u'gcs1', u'True'], [u'20181102084000', u'4292', u'gcs1', u'True'],
                  [u'20181103072000', u'4292', u'gcs1', u'True'], [u'20181102144000', u'4292', u'gcs1', u'True'],
                  [u'20181102211000', u'4293', u'gcs1', u'True'], [u'20181103044000', u'4325', u'gcs1', u'True'],
                  [u'20181102120000', u'4399', u'gcs1', u'True'], [u'20181102204000', u'4325', u'gcs1', u'True'],
                  [u'20181102154000', u'4325', u'gcs1', u'True'], [u'20181103004000', u'4325', u'gcs1', u'True'],
                  [u'20181102112000', u'4292', u'gcs1', u'True'], [u'20181103080000', u'4399', u'gcs1', u'True'],
                  [u'20181102170000', u'4366', u'gcs1', u'True'], [u'20181103061000', u'4291', u'gcs1', u'True'],
                  [u'20181103070000', u'4366', u'gcs1', u'True'], [u'20181102233000', u'4325', u'gcs1', u'True'],
                  [u'20181103052000', u'4292', u'gcs1', u'True'], [u'20181102193000', u'4312', u'gcs1', u'True'],
                  [u'20181102101000', u'4331', u'gcs1', u'True'], [u'20181102142000', u'4292', u'gcs1', u'True'],
                  [u'20181102221000', u'4326', u'gcs1', u'True'], [u'20181102145000', u'4325', u'gcs1', u'True'],
                  [u'20181103022000', u'4325', u'gcs1', u'True'], [u'20181102123000', u'4292', u'gcs1', u'True'],
                  [u'20181103015000', u'4327', u'gcs1', u'True'], [u'20181102092000', u'4292', u'gcs1', u'True'],
                  [u'20181102085000', u'41', u'tsdn-svr1', u'True'], [u'20181103035000', u'204', u'tsdn-svr1', u'True'],
                  [u'20181102120000', u'76', u'tsdn-svr1', u'True'], [u'20181102200000', u'1231', u'tsdn-svr1', u'True'],
                  [u'20181103052000', u'285', u'tsdn-svr1', u'True'], [u'20181102094000', u'261', u'tsdn-svr1', u'True'],
                  [u'20181102152000', u'65', u'tsdn-svr1', u'True'], [u'20181102223000', u'98', u'tsdn-svr1', u'True'],
                  [u'20181102103000', u'65', u'tsdn-svr1', u'True'], [u'20181103001000', u'325', u'tsdn-svr1', u'True'],
                  [u'20181103012000', u'186', u'tsdn-svr1', u'True'], [u'20181102191000', u'168', u'tsdn-svr1', u'True'],
                  [u'20181102181000', u'88', u'tsdn-svr1', u'True'], [u'20181102190000', u'286', u'tsdn-svr1', u'True'],
                  [u'20181102133000', u'64', u'tsdn-svr1', u'True'], [u'20181103014000', u'163', u'tsdn-svr1', u'True'],
                  [u'20181102203000', u'230', u'tsdn-svr1', u'True'], [u'20181103061000', u'132', u'tsdn-svr1', u'True'],
                  [u'20181102212000', u'183', u'tsdn-svr1', u'True'], [u'20181102220000', u'152', u'tsdn-svr1', u'True'],
                  [u'20181102221000', u'304', u'tsdn-svr1', u'True'], [u'20181103063000', u'289', u'tsdn-svr1', u'True'],
                  [u'20181103043000', u'273', u'tsdn-svr1', u'True'], [u'20181102192000', u'624', u'tsdn-svr1', u'True'],
                  [u'20181102175000', u'214', u'tsdn-svr1', u'True'], [u'20181103092000', u'69', u'tsdn-svr1', u'True'],
                  [u'20181103010000', u'321', u'tsdn-svr1', u'True'], [u'20181102150000', u'543', u'tsdn-svr1', u'True'],
                  [u'20181102160000', u'126', u'tsdn-svr1', u'True'], [u'20181103044000', u'262', u'tsdn-svr1', u'True'],
                  [u'20181102170000', u'210', u'tsdn-svr1', u'True'], [u'20181102144000', u'86', u'tsdn-svr1', u'True'],
                  [u'20181102215000', u'220', u'tsdn-svr1', u'True'], [u'20181103011000', u'207', u'tsdn-svr1', u'True'],
                  [u'20181102100000', u'95', u'tsdn-svr1', u'True'], [u'20181102232000', u'36', u'tsdn-svr1', u'True'],
                  [u'20181102124000', u'181', u'tsdn-svr1', u'True'], [u'20181103072000', u'595', u'tsdn-svr1', u'True'],
                  [u'20181102211000', u'367', u'tsdn-svr1', u'True'], [u'20181103045000', u'254', u'tsdn-svr1', u'True'],
                  [u'20181103000000', u'200', u'tsdn-svr1', u'True'], [u'20181102134000', u'314', u'tsdn-svr1', u'True'],
                  [u'20181103091000', u'166', u'tsdn-svr1', u'True'], [u'20181103034000', u'102', u'tsdn-svr1', u'True'],
                  [u'20181102145000', u'238', u'tsdn-svr1', u'True'], [u'20181103030000', u'101', u'tsdn-svr1', u'True'],
                  [u'20181102230000', u'395', u'tsdn-svr1', u'True'], [u'20181103074000', u'142', u'tsdn-svr1', u'True'],
                  [u'20181102195000', u'10793', u'tsdn-svr1', u'True'], [u'20181102082000', u'198', u'tsdn-svr1', u'True'],
                  [u'20181102083000', u'229', u'tsdn-svr1', u'True'], [u'20181102143000', u'127', u'tsdn-svr1', u'True'],
                  [u'20181103084000', u'292', u'tsdn-svr1', u'True'], [u'20181102162000', u'323', u'tsdn-svr1', u'True'],
                  [u'20181102155000', u'221', u'tsdn-svr1', u'True'], [u'20181102090000', u'233', u'tsdn-svr1', u'True'],
                  [u'20181103002000', u'151', u'tsdn-svr1', u'True'], [u'20181102235000', u'159', u'tsdn-svr1', u'True'],
                  [u'20181103021000', u'152', u'tsdn-svr1', u'True'], [u'20181102182000', u'423', u'tsdn-svr1', u'True'],
                  [u'20181103022000', u'242', u'tsdn-svr1', u'True'], [u'20181102095000', u'216', u'tsdn-svr1', u'True'],
                  [u'20181102081000', u'80', u'tsdn-svr1', u'True'], [u'20181102153000', u'80', u'tsdn-svr1', u'True'],
                  [u'20181102165000', u'211', u'tsdn-svr1', u'True'], [u'20181103025000', u'455', u'tsdn-svr1', u'True'],
                  [u'20181102222000', u'230', u'tsdn-svr1', u'True'], [u'20181103024000', u'71', u'tsdn-svr1', u'True'],
                  [u'20181103053000', u'136', u'tsdn-svr1', u'True'], [u'20181102213000', u'82', u'tsdn-svr1', u'True'],
                  [u'20181102225000', u'144', u'tsdn-svr1', u'True'], [u'20181102112000', u'76', u'tsdn-svr1', u'True'],
                  [u'20181102093000', u'186', u'tsdn-svr1', u'True'], [u'20181102142000', u'284', u'tsdn-svr1', u'True'],
                  [u'20181102092000', u'156', u'tsdn-svr1', u'True'], [u'20181102105000', u'90', u'tsdn-svr1', u'True'],
                  [u'20181103050000', u'153', u'tsdn-svr1', u'True'], [u'20181102140000', u'99', u'tsdn-svr1', u'True'],
                  [u'20181102111000', u'214', u'tsdn-svr1', u'True'], [u'20181103004000', u'232', u'tsdn-svr1', u'True'],
                  [u'20181102164000', u'320', u'tsdn-svr1', u'True'], [u'20181102205000', u'87', u'tsdn-svr1', u'True'],
                  [u'20181103040000', u'245', u'tsdn-svr1', u'True'], [u'20181103064000', u'320', u'tsdn-svr1', u'True'],
                  [u'20181103013000', u'96', u'tsdn-svr1', u'True'], [u'20181102224000', u'106', u'tsdn-svr1', u'True'],
                  [u'20181102172000', u'52', u'tsdn-svr1', u'True'], [u'20181102161000', u'60', u'tsdn-svr1', u'True'],
                  [u'20181103003000', u'90', u'tsdn-svr1', u'True'], [u'20181102194000', u'10568', u'tsdn-svr1', u'True'],
                  [u'20181102151000', u'128', u'tsdn-svr1', u'True'], [u'20181102121000', u'63', u'tsdn-svr1', u'True'],
                  [u'20181102201000', u'120', u'tsdn-svr1', u'True'], [u'20181102214000', u'292', u'tsdn-svr1', u'True'],
                  [u'20181103093000', u'223', u'tsdn-svr1', u'True'], [u'20181102123000', u'122', u'tsdn-svr1', u'True'],
                  [u'20181102130000', u'167', u'tsdn-svr1', u'True'], [u'20181102184000', u'136', u'tsdn-svr1', u'True'],
                  [u'20181103065000', u'192', u'tsdn-svr1', u'True'], [u'20181103031000', u'163', u'tsdn-svr1', u'True'],
                  [u'20181103041000', u'344', u'tsdn-svr1', u'True'], [u'20181103054000', u'144', u'tsdn-svr1', u'True'],
                  [u'20181102185000', u'420', u'tsdn-svr1', u'True'], [u'20181102080000', u'176', u'tsdn-svr1', u'True'],
                  [u'20181102084000', u'72', u'tsdn-svr1', u'True'], [u'20181102104000', u'209', u'tsdn-svr1', u'True'],
                  [u'20181102154000', u'211', u'tsdn-svr1', u'True'], [u'20181103090000', u'202', u'tsdn-svr1', u'True'],
                  [u'20181103042000', u'456', u'tsdn-svr1', u'True'], [u'20181102193000', u'10890', u'tsdn-svr1', u'True'],
                  [u'20181103023000', u'141', u'tsdn-svr1', u'True'], [u'20181103085000', u'44', u'tsdn-svr1', u'True'],
                  [u'20181102174000', u'462', u'tsdn-svr1', u'True'], [u'20181102091000', u'66', u'tsdn-svr1', u'True'],
                  [u'20181103071000', u'320', u'tsdn-svr1', u'True'], [u'20181102125000', u'186', u'tsdn-svr1', u'True'],
                  [u'20181102135000', u'272', u'tsdn-svr1', u'True'], [u'20181103032000', u'132', u'tsdn-svr1', u'True'],
                  [u'20181102231000', u'194', u'tsdn-svr1', u'True'], [u'20181102110000', u'211', u'tsdn-svr1', u'True'],
                  [u'20181102115000', u'117', u'tsdn-svr1', u'True'], [u'20181103070000', u'113', u'tsdn-svr1', u'True'],
                  [u'20181102101000', u'58', u'tsdn-svr1', u'True'], [u'20181102131000', u'157', u'tsdn-svr1', u'True'],
                  [u'20181102204000', u'209', u'tsdn-svr1', u'True'], [u'20181102210000', u'324', u'tsdn-svr1', u'True'],
                  [u'20181102141000', u'86', u'tsdn-svr1', u'True'], [u'20181102132000', u'49', u'tsdn-svr1', u'True'],
                  [u'20181103075000', u'147', u'tsdn-svr1', u'True'], [u'20181103033000', u'142', u'tsdn-svr1', u'True'],
                  [u'20181102171000', u'142', u'tsdn-svr1', u'True'], [u'20181102183000', u'174', u'tsdn-svr1', u'True'],
                  [u'20181103081000', u'184', u'tsdn-svr1', u'True'], [u'20181103055000', u'224', u'tsdn-svr1', u'True'],
                  [u'20181103051000', u'70', u'tsdn-svr1', u'True'], [u'20181102114000', u'112', u'tsdn-svr1', u'True'],
                  [u'20181103083000', u'249', u'tsdn-svr1', u'True'], [u'20181103020000', u'215', u'tsdn-svr1', u'True'],
                  [u'20181102233000', u'145', u'tsdn-svr1', u'True'], [u'20181102180000', u'133', u'tsdn-svr1', u'True'],
                  [u'20181102113000', u'135', u'tsdn-svr1', u'True'], [u'20181103060000', u'148', u'tsdn-svr1', u'True'],
                  [u'20181103073000', u'82', u'tsdn-svr1', u'True'], [u'20181103062000', u'156', u'tsdn-svr1', u'True'],
                  [u'20181102122000', u'250', u'tsdn-svr1', u'True'], [u'20181103080000', u'86', u'tsdn-svr1', u'True'],
                  [u'20181102234000', u'294', u'tsdn-svr1', u'True'], [u'20181103015000', u'335', u'tsdn-svr1', u'True'],
                  [u'20181102102000', u'156', u'tsdn-svr1', u'True'], [u'20181102202000', u'296', u'tsdn-svr1', u'True'],
                  [u'20181103082000', u'189', u'tsdn-svr1', u'True'], [u'20181102163000', u'263', u'tsdn-svr1', u'True'],
                  [u'20181102173000', u'117', u'tsdn-svr1', u'True'], [u'20181103005000', u'105', u'tsdn-svr1', u'True']]
        self.assertEqual(rows, answer)

    def test_case4(self):
        rows = []
        result = self.command_excute2("CNT alg=mad by=HOST tolerance=0.5 pct=0.1")
        for i in range(len(result)) :
            rows.append([result[i]['DATETIME_10M'],result[i]['CNT'],result[i]['HOST'],result[i]['outlier']])
        answer = []
        self.assertEqual(rows, answer)

    def test_case5(self):
        rows = []
        result = self.command_excute2("CNT alg=dbscan by=HOST tolerance=0.5 pct=0.1") # error check.
        for i in range(len(result)) :
            rows.append([result[i]['DATETIME_10M'],result[i]['CNT'],result[i]['HOST'],result[i]['outlier']])
        answer = []
        self.assertEqual(rows, answer)

