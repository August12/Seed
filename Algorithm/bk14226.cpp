#include <stdio.h>
#include <queue>

using namespace std;

typedef struct {
	int curCount;
	int clipConunt;
	int time;
}NODE;

queue <NODE> que;
int visit[1001][1001];
int S, answer;

void insert(int curCount, int clipConunt, int time) {
	NODE tmp;
	tmp.curCount = curCount;
	tmp.clipConunt = clipConunt;
	tmp.time = time;
	que.push(tmp);
}

int main(void) {
	scanf("%d",&S);
	insert(1,0,0);
	visit[1][0] = 1;
	while(!que.empty()) {
		int cur = que.front().curCount;
		int clip = que.front().clipConunt;
		int time = que.front().time;
		que.pop();

		if(cur == S) {
			answer = time;
			break;
		}

		if(cur > 0 && cur < 2000) {
			if(visit[cur][cur] != 1) {
				visit[cur][cur] = 1;
				insert(cur,cur,time+1);
			}

			if(visit[cur-1][clip] != 1) {
				visit[cur-1][clip] = 1;
				insert(cur-1,clip,time+1);
			}
		}

		if(cur > 0 && cur+clip < 2000) {
			if(visit[cur+clip][clip] != 1) {
				visit[cur+clip][clip] = 1;
				insert(cur+clip,clip,time+1);
			}
		}
	}
	printf("%d\n", answer);
}


