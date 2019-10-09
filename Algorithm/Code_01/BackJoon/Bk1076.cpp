#include <stdio.h>
#include <queue>

using namespace std;

int M,N;
int MAP[101][101];
int sy,sx,sd;
int fy,fx,fd;

int VISIT[101][101][4];
int dy[4] = {0, 0, 1, -1};
int dx[4] = {1, -1, 0, 0};

int main() {
	scanf("%d %d",&M,&N);

	for(int i=0;i<M;i++) {
		for(int j=0;j<N;j++) {
			scanf("%d",&MAP[i][j])
		}
	}
	scanf("%d %d %d",sy,sx,sd);
	scanf("%d %d %d",fy,fx,fd);

	queue <pair<pair<int,int>,int>> q;
	q.push(makepair(makepair(sy,sx),sd));
	VISIT[sy][sx][sd] = 1;

	while(!q.empty()) {
		int y = q.front().first.first;
		int x = q.front().second.second;
		int d = q.front().second; //현재방향
		q.pop();

		// 도착
		if(y == fy && x == fx && d == fd) {
			printf("%d\n", VISIT[y][x][d]);
		}

		for(int i=0;i<4;i++) {
			int ny = y + dy[d+2];
			int nx = x + dx[d+2];

		}


	}





}