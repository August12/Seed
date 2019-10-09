#include <stdio.h>

typedef struct {
	int quti;
	int dir;
	int max_quti;
}bug;

int T, N, M, K;
bug map[101][101];
bug cpy[101][101];

int dr[5] = {0, -1, 1, 0, 0};
int dc[5] = {0, 0, 0, -1, 1};

void init(void) {
	for(int r=0; r<N; r++) {
		for(int c=0; c<N; c++) {
			map[r][c].quti = 0;
			map[r][c].dir = 0;
			map[r][c].max_quti = 0;
			cpy[r][c] = map[r][c];
		}
	}
	return;
}

void move(void) {
	for(int r=0; r<N; r++) {
		for(int c=0; c<N; c++) {
			if(map[r][c].quti > 0) {
				int nr = r + dr[map[r][c].dir];
				int nc = c + dc[map[r][c].dir];
				int quti = map[r][c].quti;

				if(nr==0 || nr==N-1 || nc==0 || nc==N-1) {
					if(quti/2 == 0) continue;

					cpy[nr][nc].quti = quti/2;
					cpy[nr][nc].max_quti = cpy[nr][nc].quti;
					if(map[r][c].dir == 1) cpy[nr][nc].dir = 2;
					else if(map[r][c].dir == 2) cpy[nr][nc].dir = 1;
					else if(map[r][c].dir == 3) cpy[nr][nc].dir = 4;
					else cpy[nr][nc].dir = 3;
				}
				else {
					if(cpy[nr][nc].max_quti < quti) {
						cpy[nr][nc].max_quti = quti;
						cpy[nr][nc].dir = map[r][c].dir;
					}
					cpy[nr][nc].quti += quti;
				}
			}
		}
	}
}

void cpyMap(void) {
	for(int r=0; r<N; r++) {
		for(int c=0; c<N; c++) {
			map[r][c] = cpy[r][c];
			cpy[r][c].quti = 0;
			cpy[r][c].dir = 0;
			cpy[r][c].max_quti = 0;
		}
	}
	return;
}

void isAnswer(int tc) {
	long long result = 0;
	for(int r=0; r<N; r++) {
		for(int c=0; c<N; c++) {
			if(map[r][c].quti > 0) result += map[r][c].quti;
		}
	}
	printf("#%d %lld\n",tc,result);
	return;
}

int main(void) {
	//freopen("sample_input.txt", "r", stdin); 

	scanf("%d",&T);
	for(int tc=1; tc<=T; tc++) {
		scanf("%d %d %d",&N,&M,&K);
		init();
		for(int bug=1; bug<=K; bug++) {
			int r, c, quti, dir;
			scanf("%d %d %d %d",&r,&c,&quti,&dir);
			map[r][c].quti = quti;
			map[r][c].dir = dir;
			map[r][c].max_quti = quti;
		}

		for(int t=M; t>0; t--) {
			move();
			cpyMap();
		}
		isAnswer(tc);
	}
}
