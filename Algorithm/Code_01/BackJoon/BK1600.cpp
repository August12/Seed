/*
	19.3.4
	1. k 번까지 말의 움직임으로 이동.
	2. k 번 이후부터 bfs.
*/

#include <stdio.h>
#include <queue>

using namespace std;

int K, H, W;
int Map[201][201];
int Vit[201][201][31];
int Ans;
int my[4] = {1, 0, -1, 0};
int mx[4] = {0, 1, 0, -1};
int hy[8] = {2, 2, -2, -2, 1, -1, 1, -1};
int hx[8] = {1, -1, 1, -1, 2, 2, -2, -2};

queue < pair<pair<pair<int,int>,int>,int> > que;

int main() {
	scanf("%d", &K);
	scanf("%d %d", &W, &H);

	for(int i=1; i<=H; i++) {
		for(int j=1; j<=W; j++) {
			scanf("%d", &Map[i][j]);
		}
	}

	Ans = -1;
	que.push(make_pair(make_pair(make_pair(1,1),1),0));
	Vit[1][1][1] = 1;
	while(!que.empty()) {
		int y = que.front().first.first.first;
		int x = que.front().first.first.second;
		int k = que.front().first.second;
		int cnt = que.front().second;
		que.pop();

		if(y == H && x == W) {
			if(Ans > cnt || Ans == -1) {
				Ans = cnt;
			}
			break;
		}

		if(k <= K) {
			for(int i=0; i<8; i++) {
				int _y = y + hy[i];
				int _x = x + hx[i];

				if(_y>=1 && _y<=H && _x>=1 && _x<=W) {
					if(Map[_y][_x] == 0 && Vit[_y][_x][k+1] == 0) {
						que.push(make_pair(make_pair(make_pair(_y,_x),k+1),cnt+1));
						Vit[_y][_x][k+1] = 1;
					}
				}
			}
		}

		for(int i=0; i<4; i++) {
			int _y = y + my[i];
			int _x = x + mx[i];

			if(_y>=1 && _y<=H && _x>=1 && _x<=W) {
				if(Map[_y][_x] == 0 && Vit[_y][_x][k] == 0) {
					que.push(make_pair(make_pair(make_pair(_y,_x),k),cnt+1));
					Vit[_y][_x][k] = 1;
				}
			}
		}
	}

	printf("%d\n", Ans);
} 