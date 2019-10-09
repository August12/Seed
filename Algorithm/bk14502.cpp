#include <stdio.h>
#include <queue>
#include <vector>

using namespace std;

int N, M;
int CPY[9][9];
int MAP[9][9];
int dr[4] = {1, 0, -1, 0};
int dc[4] = {0, -1, 0, 1};
int Ans;

queue < pair<int,int> > q;
vector < pair<int,int> > v;

void virus() {
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			CPY[r][c] = MAP[r][c];
		}
	}
	for(int i=0; i<v.size(); i++) {
		q.push(make_pair(v[i].first,v[i].second));
	}

	while(!q.empty()) {
		int r = q.front().first;
		int c = q.front().second;
		q.pop();

		for(int i=0; i<4; i++) {
			int nr = r + dr[i];
			int nc = c + dc[i];

			if(nr>=1 && nr<=N && nc>=1 && nc<=M) {
				if(CPY[nr][nc] == 0) {
					CPY[nr][nc] = 2;
					q.push(make_pair(nr,nc));
				}
			}
		}
	}

	int cnt = 0;
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			if(CPY[r][c] == 0) cnt++;
		}
	}

	Ans = Ans < cnt ? cnt : Ans;
}

void wall(int sr, int sc, int limit) {
	if(limit == 3) {

		virus();
	}
	else {
		for(int r=sr; r<=N; r++) {
			for(int c=1; c<=M; c++) {
				if(MAP[r][c] == 0) {
					MAP[r][c] = 1;
					wall(r,c,limit+1);
					MAP[r][c] = 0;
				}
			}
		}
	}
}

int main() {
	scanf("%d %d", &N, &M);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			scanf("%d", &MAP[r][c]);
			if(MAP[r][c] == 2) {
				v.push_back(make_pair(r,c)); // 바이러스 지역
			}
		}
	}

	wall(1,1,0);
	printf("%d\n", Ans);
}