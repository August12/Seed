import os
import glob
import pandas as pd
import numpy as np
import tensorflow as tf

from sklearn.preprocessing import StandardScaler,MinMaxScaler
from sklearn.model_selection import train_test_split

def rawToOneHot(series):
    one_hot = pd.get_dummies(series)#.values
    one_hot = one_hot.rename(lambda x: series.name+'_'+x, axis=1)
    return one_hot

def rawToIndex(col):
    label = col.unique()
    one_hot = pd.get_dummies(col).values
    idx = np.argmax(one_hot, axis=1)
    return label, idx

def nanToMean(df):
    # credit_card
    df['age'].fillna(df['age'].mean(),inplace=True)
    df['since_first_date'].fillna(df['since_first_date'].mean(),inplace=True)
    df['res_code'].fillna(df['res_code'].mean(),inplace=True)
    df['income'].fillna(df['income'].mean(),inplace=True)

def preprocessing(filenames,isTraining=True,label='credit_card'):
    # make dataFrame list of each files
    raw_datas = [pd.read_csv(f) for f in filenames]

    train_data = pd.concat(raw_datas[:-1],ignore_index=True)
    if isTraining:
        label_data = raw_datas[-1][label]
    else:
       label_data = None

    train_categorical_data_1 = train_data[['id','active','saving_account','guarantees',
                                   'payroll','junior','mortgage','shortdeposit','mediumdeposit',
                                   'longdeposit','pensions','taxes','securities','homeacc','directdebit']]
    train_categorical_data_2 = train_data[['employee','gender','res_index','for_idx','channel','cate']]
    train_continuous_data = train_data[['age','since_first_date','res_code','income']]
    train_label_data = train_data[['fund','eaccount','loans','credit_card']]


    col_names = train_categorical_data_2.columns
    for col in col_names:
#         print(col)
        series = train_categorical_data_2[col]
        label, idx = rawToIndex(series)
        train_categorical_data_2[col] = idx

    train_preprocessed_data = pd.concat([train_categorical_data_1,train_categorical_data_2,
                                         train_continuous_data,train_label_data],axis=1)
    nanToMean(train_preprocessed_data)

    train_preprocessed_data.drop('channel',axis=1,inplace=True)
    train_preprocessed_data.drop('employee',axis=1,inplace=True)

    minMaxScaler = MinMaxScaler()

    mm_data = pd.DataFrame(minMaxScaler.fit_transform(train_preprocessed_data[['age','since_first_date','res_code','income']]),
                columns=['age','since_first_date','res_code','income'])
    for col in mm_data.columns:
        train_preprocessed_data[col] = mm_data[col]

    train_splited_datas = []
    for i in range(5):
        train_splited_datas.append(train_preprocessed_data[572610*i:572610*(i+1)].reset_index(drop=True))

    train_datas = np.array([np.array(i) for i in train_splited_datas])
    label_datas = np.array(label_data)

    return train_datas,label_datas

def makeDataForCNN(x_data,y_data=None):
    x_data = np.concatenate(x_data, axis=1)
#     print(x_data.shape)

    x_data = x_data.reshape(-1,5,26,1)
    # x_data = train_d.reshape(-1,)
    if y_data is None:
        return x_data
    
    y_data = y_data.reshape(-1,1)

#     print(x_data.shape,y_data.shape)

    zero_label_index = []
    one_label_index = []
    for idx, label in enumerate(y_data):
        if label[0] == 0:
            zero_label_index.append(idx)
        else:
            one_label_index.append(idx)

    np.random.shuffle(zero_label_index)
    zero_label_index = zero_label_index[:len(one_label_index)*2]
#     print(len(zero_label_index))

    zero_label_index.sort()
    selected_data_index = zero_label_index + one_label_index*2
#     selected_data_index.sort()
    np.random.shuffle(selected_data_index)
#     print(len(selected_data_index))

    x_data = x_data[selected_data_index]
    y_data = y_data[selected_data_index]
#     print(x_data.shape,y_data.shape)

    onehot_encoded = []
    for label in y_data:
        li = [0 for _ in range(2)]
        li[label[0]] = 1
        onehot_encoded.append(li)
    y_data = np.array(onehot_encoded)

    return (x_data,y_data)

class CNN:
    def __init__(self,batch_size=1024):
        self.batch_size = batch_size
        self.keep_prob = tf.placeholder(tf.float32)
        
        self.X = tf.placeholder(tf.float32, [None,5*26])
        self.X_img = tf.reshape(self.X, [-1,5,26,1])
        self.y = tf.placeholder(tf.float32, [None,2])

        self.W1 = tf.Variable(tf.random_normal([3,3,1,32], stddev=0.01))
#         self.W2 = tf.Variable(tf.random_normal([3,3,1,64], stddev=0.01))
        self.Whid = tf.get_variable("Whid", shape=[3*13*32,128], initializer=tf.contrib.layers.xavier_initializer())
        self.Wout = tf.get_variable("Wout", shape=[128, 2], initializer=tf.contrib.layers.xavier_initializer())
#         self.bias = tf.Variable(tf.random_normal([2]))

        self.L1 = tf.nn.conv2d(self.X_img, self.W1, strides=[1,1,1,1],padding='SAME')
#         print(self.L1)
        self.L1 = tf.nn.relu(self.L1)
        self.L1 = tf.nn.max_pool(self.L1,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
#         print(self.L1)
        self.L1 = tf.reshape(self.L1,[-1,3*13*32])
        
        
#         self.L2 = tf.nn.conv2d(self.L1, self.W2, strides=[1,1,1,1],padding='SAME')
#         print(self.L2)
#         self.L2 = tf.nn.relu(self.L2)
#         self.L2 = tf.nn.max_pool(self.L2,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
#         print(self.L2)
#         self.L2 = tf.reshape(self.L2,[-1,3*13*64])
#         print(self.L2)

        self.Lhid = tf.nn.relu(tf.matmul(self.L1,self.Whid))
        self.Lhid = tf.nn.dropout(self.Lhid, keep_prob=self.keep_prob)

        self.hypothesis = tf.matmul(self.Lhid,self.Wout)

    def fit(self,x_data,y_data):
        x_data, y_data = makeDataForCNN(x_data,y_data)
        
        x_train, x_test, y_train, y_test = train_test_split(x_data,y_data,test_size=0.2,random_state=0)

#         print(x_train.shape)
#         print(x_test.shape)
        print(y_train.shape)
        print(y_test.shape)
        
        cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits=self.hypothesis, labels=self.y))
        optimizer = tf.train.AdamOptimizer(learning_rate=0.001).minimize(cost)

        predicted = tf.argmax(self.hypothesis, axis=1)
        accuracy = tf.reduce_mean(tf.cast(tf.equal(predicted, tf.argmax(self.y, axis=1)), dtype=tf.float32))

        sess = tf.Session()
        sess.run(tf.global_variables_initializer())

        for epoch in range(11):
            avg_cost = 0
            training_batch = list(zip(range(0, len(x_train), self.batch_size),
                                     range(self.batch_size, len(x_train)+1, self.batch_size)))
            if training_batch[-1][1] < len(x_train):
                start = training_batch[-1][1]+1
                end = len(x_train)
                training_batch.append((start,end))
            
            for start,end in training_batch:
                c,_ = sess.run([cost,optimizer],
                               feed_dict={self.X_img:x_train[start:end],
                                          self.y:y_train[start:end],
                                          self.keep_prob:0.5})

            if epoch % 10 == 0:
                print('cost: {}'.format(c))

                p,a = sess.run([predicted,accuracy],
                             feed_dict={self.X_img: x_test,
                                        self.y: y_test, self.keep_prob:1})
                print('epoch: {}, predicted: {}, accuracy: {}\n'.format(epoch,p,a))

        h,p,a = sess.run([self.hypothesis,predicted,accuracy],
                     feed_dict={self.X_img: x_test,
                                self.y: y_test, self.keep_prob:1})
        print('hypo: {}'.format(h))
        print('predicted: {}, accuracy: {}\n'.format(p,a))
    def predict(self, x_data):
        x_data = makeDataForCNN(x_data)
        
        predicted = tf.argmax(self.hypothesis, axis=1)

        sess = tf.Session()
        sess.run(tf.global_variables_initializer())

        h,p = sess.run([self.hypothesis,predicted],
                     feed_dict={self.X_img: x_data,self.keep_prob:1})
        print('hypo: {}'.format(h))
        print('predicted: {}\n'.format(p))
        return h,p

path = 'data'
filenames = glob.glob(os.path.join(path,'*.csv'))

filenames.sort()
print(filenames)

x_data_with_id, y_data = preprocessing(filenames[:-1])

x_data = x_data_with_id[:,:,1:]
id_data = x_data_with_id[:,:,0][0]
model = CNN()
model.fit(x_data,y_data)

x_data_with_id, y_data = preprocessing(filenames[1:],isTraining=False)
x_data = x_data_with_id[:,:,1:]
id_data = x_data_with_id[:,:,0][0]

h,p = model.predict(x_data)
#####################여기까지가 예측이고 밑은 예측한 0,1 라벨 개수랑 7월 정답데이터랑 비교한것####################


set(p)

count0 = 0
count1 = 0
one_index=[]
for idx,i in enumerate(p):
    if i==1:
        count1+=1
        one_index.append(idx)
    else:
        count0+=1
print(count0,count1)

prediected_one = [(abs(v[1]-v[0]),i) for v,i in list(zip(h[one_index],one_index))]
prediected_one.sort(reverse=True)
prediected_one

prediected_one_idx = [i for v,i in prediected_one]
prediected_one_idx

df = pd.read_csv(filenames[-2])
june_df = df[['id','fund','eaccount','loans','credit_card']]
june_df = june_df[june_df['credit_card']==0]

tmp2=[june_df[june_df['id']==i] for i in prediected_one_idx]
result = pd.concat(tmp2,ignore_index=True)
re_idx=list(result['id'])

df3 = pd.read_csv('data/ans/result.csv')
f = df3[df3['fund']==1]
e = df3[df3['eaccount']==1]
l = df3[df3['loans']==1]
c = df3[df3['credit_card']==1]

re=[c[c['id']==i] for i in re_idx[:200]]
pd.concat(re)