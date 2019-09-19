#include <stdio.h>
#include <queue>

using namespace std;

typedef struct {
	int position;
	int time;
	queue < int > route;
}NODE;

queue < NODE > q;
int visit[200001];
int N, K;
int answer;
queue <int> answerQue;

void insert(int pos, int time, queue < int > innerQ) {
	NODE tmp;
	tmp.position = pos;
	tmp.time = time;
	tmp.route = innerQ;
	q.push(tmp);
}

int main(void) {
	scanf("%d %d",&N, &K);

	if(N >= K) {
		answer = N - K;
		for(int i = N; i >= K; i--) {
			answerQue.push(i);
		}
	}
	else {
		visit[N] = 1;
		queue <int> route;
		route.push(N);
		insert(N,0,route);
		while(!q.empty()) {
			int pos = q.front().position;
			int time = q.front().time;
			queue <int> que = q.front().route;
			queue <int> tmp2 = que;
			q.pop();

			// printf("innerQ :\n");
			// int tmp2Size = tmp2.size();
			// for(int j=0; j<tmp2Size; j++) {
			// 	printf("%d ", tmp2.front());
			// 	tmp2.pop();
			// }
			// printf("\n");

			if(pos == K) {
				answer = time;
				answerQue = que;
				break;
			}

			if(pos >= 0 && pos < 200000) {
				if(visit[pos+1] != 1) {
					visit[pos+1] = 1;
					queue <int> tmp = que;
					tmp.push(pos+1);
					insert(pos+1,time+1,tmp);
				}
			}
			if(pos > 0 && pos <= 200000) {
				if(visit[pos-1] != 1) {
					visit[pos-1] = 1;
					queue <int> tmp = que;
					tmp.push(pos-1);
					insert(pos-1,time+1,tmp);
				}
			}
			if(pos >= 0 && (pos*2) < 200000) {
				if(visit[pos*2] != 1) {
					queue <int> tmp = que;
					visit[pos*2] = 1;
					tmp.push(pos*2);
					insert(pos*2,time+1,tmp);
				}
			}
		}
	}

	printf("%d\n", answer);
	int size = answerQue.size();
	for(int i=0; i<size; i++) {
		printf("%d ", answerQue.front());
		answerQue.pop();
	}
	printf("\n");
}
