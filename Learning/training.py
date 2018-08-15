
# coding: utf-8

# In[1]:


#-*- coding: utf-8 -*-
#한글쓰기위에서 위 코드 추가
import numpy as np
import tensorflow as tf


# In[33]:


training_epochs = 500 # 학습 횟수
n_dim = 193
n_classes = 3
learning_rate = 0.01 # 학습 비율
sd = 1/np.sqrt(n_dim) # 표준 편차


# In[34]:


n_hidden_units_one = 300
n_hidden_units_two = 200
n_hidden_units_three = 100


# In[35]:


X = tf.placeholder(tf.float32,[None,n_dim]) # 입력 데이터 파라미터
Y = tf.placeholder(tf.float32,[None,n_classes]) # 정답 데이터 파라미터


# In[36]:


# 1차 히든 레이어(원소까지 랜덤인 배열 생성)
W_1 = tf.Variable(tf.random_normal([n_dim, n_hidden_units_one], mean=0, stddev=sd), name="w1")
b_1 = tf.Variable(tf.random_normal([n_hidden_units_one], mean=0, stddev=sd), name="b1")
h_1 = tf.nn.relu(tf.matmul(X, W_1) + b_1)


# In[37]:


# 2차 히든 레이어
W_2 = tf.Variable(tf.random_normal([n_hidden_units_one, n_hidden_units_two], mean=0, stddev=sd), name="w2")
b_2 = tf.Variable(tf.random_normal([n_hidden_units_two], mean=0, stddev=sd), name="b2")
h_2 = tf.nn.tanh(tf.matmul(h_1, W_2) + b_2)


# In[38]:


# 3차 히든 레이어
W_3 = tf.Variable(tf.random_normal([n_hidden_units_two, n_hidden_units_three], mean=0, stddev=sd), name="w3")
b_3 = tf.Variable(tf.random_normal([n_hidden_units_three], mean=0, stddev=sd), name="b3")
h_3 = tf.nn.relu(tf.matmul(h_2, W_3) + b_3)


# In[39]:


# 드롭아웃 과정 추가
keep_prob = tf.placeholder(tf.float32)
h_3_drop = tf.nn.dropout(h_3, keep_prob)


# In[40]:


W = tf.Variable(tf.random_normal([n_hidden_units_three, n_classes], mean=0, stddev=sd), name="w")
b = tf.Variable(tf.random_normal([n_classes], mean = 0, stddev=sd), name="b")
y_ = tf.nn.softmax(tf.matmul(h_3_drop, W) + b)

cross_entropy = -tf.reduce_sum(Y*tf.log(tf.clip_by_value(y_, 1e-10, 1.0)))
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)


# In[70]:


saver = tf.train.Saver()

# 학습이 완료되면 정답률 체크
#correct_prediction = tf.equal(tf.argmax(y_, 1), tf.argmax(Y, 1))
#accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

#saver = tf.train.Saver()

correct_prediction = tf.equal(tf.argmax(y_, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

with tf.Session() as sess:
    sess.run(tf.initialize_all_variables())
    sound_data = np.load('workspace/sound_data.npz')
    test_data = np.load('workspace/test_data.npz')
    X_data = sound_data['X']
    y_data = sound_data['y']
    X_test = test_data['X']
    
    for epoch in range(training_epochs):
        sess.run(train_step, feed_dict={X: X_data, Y: y_data, keep_prob: 0.5 })
        
        if epoch%100 == 0 :
            train_accuracy = sess.run(accuracy, feed_dict={X: X_data, Y: y_data, keep_prob: 1.0})
            print("step %d, training accuracy %g"%(epoch, train_accuracy))
    
    
    
    print(sess.run(y_, feed_dict={X: X_test, keep_prob: 1.0}))

    
    
    
    

