
# coding: utf-8

# In[1]:


import glob
import librosa
import numpy as np
import matplotlib.pyplot as plt
import librosa.display

def extract_feature(file_name):
    X, sample_rate = librosa.load(file_name)
    stft = np.abs(librosa.stft(X))
    mfccs = np.mean(librosa.feature.mfcc(y=X, sr=sample_rate, n_mfcc=40).T,axis=0)
    chroma = np.mean(librosa.feature.chroma_stft(S=stft, sr=sample_rate).T,axis=0)
    mel = np.mean(librosa.feature.melspectrogram(X, sr=sample_rate).T,axis=0)
    contrast = np.mean(librosa.feature.spectral_contrast(S=stft, sr=sample_rate).T,axis=0)
    tonnetz = np.mean(librosa.feature.tonnetz(y=librosa.effects.harmonic(X), sr=sample_rate).T,axis=0)
    return mfccs,chroma,mel,contrast,tonnetz

def parse_audio_files(filenames):
    rows = len(filenames)
    features, labels = np.zeros((rows,193)), np.zeros((rows,3))
    i = 0
    for fn in filenames:
        try:
            mfccs, chroma, mel, contrast,tonnetz = extract_feature(fn)
            ext_features = np.hstack([mfccs,chroma,mel,contrast,tonnetz])
            if ((fn.split('/')[1].split('_')[0]) == 'baby'):
                y_col = 0
            elif ((fn.split('/')[1].split('_')[0]) == 'dog'):
                y_col = 1
            else:
                y_col = 2
        except:
            print(fn)
        else:
            features[i] = ext_features
            labels[i, y_col] = 1
            i += 1
    return features, labels


audio_files = []
audio_files.extend(glob.glob('workspace/*.wav'))

X, y = parse_audio_files(audio_files)
np.savez('workspace/sound_data.npz',X=X,y=y)



    

