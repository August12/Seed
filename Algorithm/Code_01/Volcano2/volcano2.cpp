/*
	1. 바리게이트를 세운다.
	2. 넓이우선으로 완전 탐색을 한다.
	3. 답과 비교한다.
	4. 1~3과정을 모든 바리게이트를 세우는 경우의 수로 반복한다.
*/
#include <stdio.h>
#include <queue>
#include <vector>

using namespace std;

int T, N, M;
int Map[7][7];
int dy[4] = {1, 0, -1, 0};
int dx[4] = {0, -1, 0, 1};
int Ans;

queue < pair<int,int> > que;
vector < pair<int,int> > vec;

void bfs() {
	int cpy_Map[7][7];
	int num = 0;

	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			cpy_Map[i][j] = Map[i][j];
			if(Map[i][j] == 2) {
				que.push(make_pair(i,j));
			}
		}
	}

	while(!que.empty()) {
		int now_y = que.front().first;
		int now_x = que.front().second;
		cpy_Map[now_y][now_x] = -1;
		que.pop();

		for(int i=0; i<4; i++) {
			int _y = now_y + dy[i];
			int _x = now_x + dx[i];

			if(_y>=1 && _y<=N && _x>=1 && _x<=N) {
				if(cpy_Map[_y][_x] == 0) {
					que.push(make_pair(_y,_x));
				}
			}
		}
	}

	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			printf("%d ", cpy_Map[i][j]);
			if(cpy_Map[i][j] == 0) {
				num++;
			}
		}
		printf("\n");
	}

	if(Ans < num) {
		Ans = num;
	}
}

void dfs(int now, int cnt) {
	if(cnt == M) {
		bfs();
	}
	else {
		for(int i=now; i<N*N; i++) {
			int nxt_y = (i/N)+1;
			int nxt_x = (i%N)+1;

			if(Map[nxt_y][nxt_x] == 0) {
				Map[nxt_y][nxt_x] = 1;
				dfs(i+1,cnt+1);
				Map[nxt_y][nxt_x] = 0;
			}
		}
	}
}

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d", &N, &M);

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				Map[i][j] = 0;
			}
		}
		Ans = 0;

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				scanf("%d", &Map[i][j]);
			}
		}

		dfs(0,0);
		printf("#%d %d\n", test_case, Ans);

	}
}