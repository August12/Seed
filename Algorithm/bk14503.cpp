#include <stdio.h>
#include <stdlib.h>

int N, M;
int sr, sc, sd;
int MAP[50][50];
int Ans;
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

int main() {
	scanf("%d %d", &N, &M);
	scanf("%d %d %d", &sr, &sc, &sd);
	for(int r=0; r<N; r++) {
		for(int c=0; c<M; c++) {
			scanf("%d", &MAP[r][c]);
		}
	}

	MAP[sr][sc] = 2;
	int nd = sd;
	while(true) {

		for(int i=0; i<4; i++) {
			nd = (nd+3)%4;
			int nr = sr + dr[nd];
			int nc = sc + dc[nd];

			if(nr>=0 && nr<N && nc>=0 && nc<M) {
				if(MAP[nr][nc] == 0) {
					sr = nr;
					sc = nc;
					MAP[sr][sc] = 2;
					i = -1;
					continue;
				}
			}
		}

		int fd = (nd+2)%4;
		sr = sr + dr[fd];
		sc = sc + dc[fd];
		if(MAP[sr][sc] == 1) break;
	}

	for(int r=0; r<N; r++) {
		for(int c=0; c<M; c++) {
			if(MAP[r][c] == 2) Ans++;
		}
	}

	printf("%d\n", Ans);
}