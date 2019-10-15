#include <stdio.h>
#include <queue>
#include <algorithm>

using namespace std;

int N, M, T;
queue <pair<pair<int, int>,int> > q;
int map[50][50];
int vit[50][50];
int pip[8][4] = {
	{0,0,0,0},
	{1,1,1,1},
	{1,0,1,0},
	{0,1,0,1},
	{1,1,0,0},
	{0,1,1,0},
	{0,0,1,1},
	{1,0,0,1}
};
int dy[4] = {-1,0,1,0};
int dx[4] = {0,1,0,-1};
int answer;

int main(void) {
	freopen("sample_input.txt","r",stdin);

	scanf("%d", &T);
	for(int tc=1; tc<=T; tc++) {
		int sy, sx, time;
		answer = 0;
		scanf("%d %d %d %d %d", &N, &M, &sy, &sx, &time);
		for(int y=0; y<N; y++) {
			for(int x=0; x<M; x++) {
				scanf("%d",&map[y][x]);
				vit[y][x] = 0;
			}
		}
		while(!q.empty()) {
			q.pop();
		}

		answer++;
		vit[sy][sx] = 1;
		q.push(make_pair(make_pair(sy,sx),time-1));
		//printf("%d\n", time-1);
		while(!q.empty()) {
			int y = q.front().first.first;
			int x = q.front().first.second;
			int pipe = map[y][x];
			int t = q.front().second;
			//printf("%d %d %d %d\n", y, x, t, answer);
			q.pop();

			if(t == 0) break;

			for(int i=0; i<4; i++) {
				int ny = y + dy[i];
				int nx = x + dx[i];
				
				//printf("%d\n", i);

				if(ny<0 && nx<0 && ny>N-1 && nx>M-1) continue;
				//printf("a\n");

				int npipe = map[ny][nx];
				if(map[ny][nx] == 0) continue;
				//printf("%d %d\n",ny,nx);

				if(vit[ny][nx] == 0) {
					//printf("asdfa\n");
					if(pip[pipe][i] == 1 && pip[npipe][(i+2)%4] == 1) {
						//printf("\n");
						vit[ny][nx] = 1;
						answer++;
						q.push(make_pair(make_pair(ny,nx),t-1));
					}
				}
			}
		}
		printf("#%d %d\n", tc, answer);
	}
}