#include <stdio.h>

int T, N;
int MAP[101][101];
int dy[4] = {-1, 1, 0, 0};
int dx[4] = {0, 0, 1, -1};
int CNT;

void dfs(int y,int x) {
	for(int i=0; i<4; i++) {
		int ny = y + dy[i];
		int nx = x + dx[i];

		if(ny>0 && nx>0 && ny<=N && nx<=N) {
			if(MAP[ny][nx] == 0) {
				MAP[ny][nx] = -1;
				dfs(ny, nx);
			}
		}
	}
}

int main(void) {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d", &N);
		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				MAP[i][j] = 0;
			}
		}
		CNT = 0;

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				scanf("%d", &MAP[i][j]);
			}
		}

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				if(MAP[i][j] == 2)
					dfs(i, j);
			}
		}

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				if(MAP[i][j] == 0)
					CNT++;
			}
		}

		printf("#%d %d\n", test_case, CNT);
	}
}