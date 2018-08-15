import os
from flask import Flask, request, redirect, url_for
from werkzeug import secure_filename


app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = './upload'
app.config['MAX_CONTENT_LENGTH'] = 32 * 1024 * 1024 * 10

@app.route('/uploads', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        file = request.files['file']
        if file :
            filename = file.filename
            audio_file=os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(audio_file)
            mfccs = extract_feature(audio_file)
            np.array(mfccs)
            print("mfcc len : " +str(mfccs.size))
            x_data = np.hstack([mfccs])
            print(x_data,x_data.size)
            print(x_data.shape)
            #
            y_hat = sess.run(y_, feed_dict={X: x_data})
            ans_list = np.argmax(y_hat,1)
            ans_p1 = np.zeros(shape=[10], dtype=np.float32)
            ans_p2 = np.zeros(shape=[10], dtype=np.float32)
            ans_1sec = np.zeros(shape=[10], dtype=np.float32)
            ans_fin = np.zeros(shape=[10], dtype=np.float32)
            #
            j=0 # 아래 for문을 위한 index
            for k in ans_list:
                ans_p2[k] += 1 # 정답을 모은다.
                if(j%(win_size/2) == 0):
                    # 25개(0.5초, Sliding size)를 모았으면, 50개(1초 = Window Size)에 대한 정답을 낸다.
                    ans_1sec = ans_p1 + ans_p2
                    ans_1sec /= ans_1sec.sum()
                    # 최종 정답에 50개의 정답 결과를 반영한다.
                    ans_fin[ans_1sec.argmax()] += 1
                    # 한 칸을 슬라이드 시킨다.
                    ans_p1 = ans_p2
                    ans_p2 = np.zeros(shape=[10], dtype=np.float32)
                j += 1
            # 맨 마지막에 나온 값도 처리를 해줘야한다.
            ans_1sec = ans_p1 + ans_p2
            ans_1sec /= ans_1sec.sum()
            ans_fin[ans_1sec.argmax()] += 1
            ans_fin /= ans_fin.sum()
            # 추측한 정답을 출력한다.
            with printoptions(precision=3, suppress=True):
                print(ans_fin)
            index = ans_fin.argmax()
            print(index)       
            #index = 0
            #
            return jsonify(classification=index.tolist(),prob=ans_fin.tolist())
    return '''
    <!doctype html>
        <title>Upload new File</title>
        <h1>Upload new File</h1>
    <form action="" method=post enctype=multipart/form-data>
    <p><input type=file name=file>
    <input type=submit value=Upload>
    </form>
    '''


app.run(host='0.0.0.0',port=8080)