# Anomalies

**anomalies 명령어 문법 및 연동규격 설명서 입니다**

<br/>

### A. 개요

비정상적인 값을 찾아내는 기능입니다.

</br>

### B. 설명

Input으로 받은 Dataframe에 선택한 이상탐지 알고리즘으로 특정 임계값 범위를 벗어나는 값에 flag값을 붙여서 Dataframe을 반환합니다.

anomalies 따로 Detect 값만 뽑아 낼 수 있는 기능이 있습니다.

</br>

### C. Parameters

```
... | anomalies index target [alg=basic] [bound=2] [by=field_name]

... | anomalies index target [alg=basic] [bound=2] [by=field_name] [direct=both] 	      								[alert_window=last_60s]
```

`index` : 시계열 데이터 필드명 입니다. **반드시** 지정해 줘야 합니다.

`target` : 분석할 데이터 필드명 입니다. **반드시** 지정해 줘야 합니다.

`alg` : **basic**, **agile**, **robust** 알고리즘을 선택합니다. 각각 기본통계, SARIMA, STL decomposition을 수행합니다. 기본값 = **basic**.

- **basic** : 단순 통계적 방법을 사용하였습니다. 1.959964는 신뢰구간 95% z상수 값입니다.
  $$
  \bar x \pm \frac{1.959964 \times s}{\sqrt n}
  $$

  ​	    upper와 lower를 구하여 이상치를 판단합니다.

- **agile** : SARIMA 모델을 사용한 알고리즘입니다. 모델의 예측 값과 잔차 값을 1차 계산하고 계산된 값을 이용해서 basic알고리즘으로 임계값을 구하여 이상치를 판단합니다.
- **robust** : Seasonal_Decomposition을 사용한 알고리즘입니다. 계절성, 추세, 잔차 값을 구별하여 잔차 값으로 임계값을 구하여 이상치를 판단합니다.

`bound` : 임계값 범위의 scale을 지정합니다. 위의 수식에 값의 범위를 늘리는 데 사용 합니다. 기본값 = 2.

`by` : 그룹으로 각각의 이상탐지를 시행할 수 있습니다. 기본값 = None

----

**direct**와 **alert_window** 파라미터가 들어오면 return 값은 이상 값만 검출됩니다.

`direct` : 이상 값을 찾을 범위, 위-아래-둘다 를 선택할 수 있습니다.

`alert_window` : 최근 시간 범위 내 이상치 값을 탐지합니다.
​			      last_60s이면 최근 60초 이내 데이터 중 이상치를 탐지합니다.
​			      last_1m이면 최근 1분 이내 데이터 중 이상치를 탐지합니다.
​			      last_1h이면 최근 1시간 이내 데이터 중 이상치를 탐지합니다.

</br>

### D. Examples

- 데이터가 다음과 같이 존재합니다.

|    time    | value |
| :--------: | ----- |
| 2011-11-1  | 5     |
| 2011-11-2  | 10    |
| 2011-11-3  | 15    |
| 2011-11-4  | 20    |
|     .      | .     |
|     .      | .     |
|     .      | .     |
| 2011-11-16 | 95    |
| 2011-11-17 | 300   |
| 2011-11-18 | 105   |
|     .      | .     |
|     .      | .     |
| 2011-11-29 | 160   |
| 2011-11-30 | 165   |

<br/>

- basic 알고리즘을 사용 하는 예

  ~~~
  ... | anomalies time value
  ... | anomalies time value alg=basic bound=2
  ~~~

  명령어 이후 DataFrame

  |    time    | value | upper     | lower     | anomaly |
  | :--------: | ----- | --------- | --------- | ------- |
  | 2011-11-1  | 5     | threshold | threshold | False   |
  | 2011-11-2  | 10    | threshold | threshold | False   |
  | 2011-11-3  | 15    | threshold | threshold | False   |
  | 2011-11-4  | 20    | threshold | threshold | False   |
  |     .      | .     | .         | .         | .       |
  |     .      | .     | .         | .         | .       |
  |     .      | .     | .         | .         | .       |
  | 2011-11-16 | 95    | threshold | threshold | False   |
  | 2011-11-17 | 300   | threshold | threshold | True    |
  | 2011-11-18 | 105   | threshold | threshold | False   |
  |     .      | .     | .         | .         | .       |
  |     .      | .     | .         | .         | .       |
  | 2011-11-29 | 160   | threshold | threshold | False   |
  | 2011-11-30 | 165   | threshold | threshold | False   |

  <br/>

- agile 알고리즘을 사용 하는 예

  ~~~
  ... | anomalies time value alg=agile
  ~~~

  명령어 이후 DataFrame

  |    time    | value | predict | residuals | upper     | lower     | anomaly |
  | :--------: | ----- | ------- | --------- | --------- | --------- | ------- |
  | 2011-11-1  | 5     | NaN     | 5         | threshold | threshold | False   |
  | 2011-11-2  | 10    | 4       | 6         | threshold | threshold | False   |
  | 2011-11-3  | 15    | 8       | 7         | threshold | threshold | False   |
  | 2011-11-4  | 20    | 14      | 6         | threshold | threshold | False   |
  |     .      | .     | .       | .         | .         |           |         |
  |     .      | .     | .       | .         | .         |           |         |
  |     .      | .     | .       | .         | .         |           |         |
  | 2011-11-16 | 95    | 91      | 4         | threshold | threshold | False   |
  | 2011-11-17 | 300   | 97      | 203       | threshold | threshold | True    |
  | 2011-11-18 | 105   | 298     | 193       | threshold | threshold | True    |
  |     .      | .     | .       | .         | .         |           |         |
  |     .      | .     | .       | .         | .         |           |         |
  | 2011-11-29 | 160   | 156     | 4         | threshold | threshold | False   |
  | 2011-11-30 | 165   | 160     | 5         | threshold | threshold | False   |

- robust 알고리즘을 사용 하는 예

  ~~~
  ... | anomalies time value alg=robust
  ~~~

  명령어 이후 DataFrame

  |    time    | value | residuals | upper     | lower     | anomaly |
  | :--------: | ----- | --------- | --------- | --------- | :-----: |
  | 2011-11-1  | 5     | NaN       | threshold | threshold |  False  |
  | 2011-11-2  | 10    | NaN       | threshold | threshold |  False  |
  | 2011-11-3  | 15    | 1.062     | threshold | threshold |  False  |
  | 2011-11-4  | 20    | 1.053     | threshold | threshold |  False  |
  |     .      | .     | .         | .         | .         |    .    |
  |     .      | .     | .         | .         | .         |    .    |
  |     .      | .     | .         | .         | .         |    .    |
  | 2011-11-16 | 95    | 0.690     | 1.229     | 0.567     |  False  |
  | 2011-11-17 | 300   | 1.716     | 1.361     | 0.700     |  True   |
  | 2011-11-18 | 105   | 0.717     | 1.333     | 0.672     |  False  |
  |     .      | .     | .         | .         | .         |    .    |
  |     .      | .     | .         | .         | .         |    .    |
  | 2011-11-29 | 160   | NaN       | threshold | threshold |  False  |
  | 2011-11-30 | 165   | NaN       | threshold | threshold |  False  |

<br/>

- detection을 수행 했을 때, 해당 알고리즘으로 이상 값만 반환.

  ~~~
  ... | anomalies time value alg=basic direct=both
  ~~~

  명령어 이후 Dataframe

  |    time    | value | upper       | lower      | anomaly |
  | :--------: | ----- | ----------- | ---------- | ------- |
  | 2011-11-17 | 300   | 282.8897323 | 23.1286923 | True    |

- detection 방향이 below 일때, lower 값 보다 낮은 이상 값만을 찾고 Last_60s 임으로 빈 dataframe반환

  ~~~
  ... | anomalies time value alg=basic direct=below window=last_60s
  ~~~

  명령어 이후 Dataframe

  | time  | value | upper | lower | anomaly |
  | :---: | :---: | :---: | :---: | :-----: |
  | Empty | Empty | Empty | Empty |  Empty  |