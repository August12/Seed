#include <stdio.h>
#include <stdlib.h>

int main() {
	int T;
	int N,M;
	int Y,X,D;
	int Map[51][51];
	int dy[4] = {-1, 0, 1, 0};
	int dx[4] = {0, 1, 0, -1};

	freopen("Input_RobotRat.txt","r",stdin);

	scanf("%d",&T);

	for(int test_case=0;test_case<T;test_case++) {
		int answer = 0;

		scanf("%d %d",&N,&M);
		scanf("%d %d %d",&Y,&X,&D);

		for(int i=1;i<=N;i++) {
			for(int j=1;j<=M;j++) {
				scanf("%d",&Map[i][j]);
			}
		}

		// solutions
		while(true) {
			Map[Y][X] = 2;
			int nd = D;
			int ny = Y;
			int nx = X;
			for(int i=0;i<4;i++) {
				ny = Y + dy[nd];
				nx = X + dx[nd];

				if(Map[ny][nx]==0) {
					break;
				}
				else {
					nd = (nd + 1) % 4;
				}
			}

			if(Map[ny][nx]==0) {
				answer++;
				Y = ny;
				X = nx;
				D = nd;
			}
			else {
				int pd = (D + 2) % 4;
				int py = Y + dy[pd];
				int px = X + dx[pd];
				if(Map[py][px]==1) {
					break;
				}
				else {
					answer++;
					Y = py;
					X = px;
				}
			}
		}
		printf("#%d %d\n", test_case,answer);	
	}
}