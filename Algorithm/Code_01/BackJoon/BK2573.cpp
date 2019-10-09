/*
	1. bfs로 빙산 덩어리 개수를 확인한다.
	2. 탐색을 진행하면서 네 방향을 확인 0 갯수 만큼 현재 위치에서 뺀다.
*/

#include <stdio.h>
#include <queue>

using namespace std;

int N, M;
int Map[301][301];
int Vit[301][301];
int Ans, Pic;
int dy[4] = {1, 0, -1, 0};
int dx[4] = {0, 1, 0, -1};
int Cpymap[301][301];

queue < pair<int,int> > q;

void bfs() {
	while(!q.empty()) {
		int y = q.front().first;
		int x = q.front().second;
		q.pop();

		for(int i=0; i<4; i++) {
			int _y = y + dy[i];
			int _x = x + dx[i];

			if(_y >= 1 && _x >=1 && _y <= N && _x <= M) {
				if(Map[_y][_x] != 0 && Vit[_y][_x] == 0) {
					q.push(make_pair(_y,_x));
					Vit[_y][_x] = 1;
				}
			}
		}
	}
	Pic++;
}

int main() {
	scanf("%d %d", &N, &M);
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=M; j++) {
			scanf("%d", &Map[i][j]);
		}
	}

	while(true) {

		Pic = 0;
		for(int i=1; i<=N; i++) {
			for(int j=1; j<=M; j++) {
				Cpymap[i][j] = Map[i][j];
				if(Cpymap[i][j] != 0 && Vit[i][j] == 0) {
					q.push(make_pair(i,j));
					Vit[i][j] = 1;
					bfs();
				}
			}
		}

		if(Pic >= 2) {
			break;
		}
		else if(Pic == 0) {
			Ans = 0;
			break;
		}
		else {
			for(int i=1; i<=N; i++) {
				for(int j=1; j<=M; j++) {

					if(Map[i][j] != 0) {
						for(int k=0; k<4; k++) {
							int _i = i + dy[k];
							int _j = j + dx[k];

							if(_i >= 1 && _i <= N && _j >= 1 && _j <= M) {
								if(Map[_i][_j] == 0) {
									if(Cpymap[i][j]-1 < 0) Cpymap[i][j] = 0;
									else Cpymap[i][j] = Cpymap[i][j] - 1;
								}
							}
						}
					}
				}
			}
			Ans++;

			for(int i=1; i<=N; i++) {
				for(int j=1; j<=M; j++) {
					Map[i][j] = Cpymap[i][j];
					Vit[i][j] = 0;
				}
			}

			while(!q.empty()) q.pop();
		}
	}

	printf("%d\n", Ans);
}