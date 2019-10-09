#include <stdio.h>

int N, M;
int MAP[501][501];
int VIT[501][501];
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};
int Ans;

void make(int r, int c, int cnt, int sum) {
	if(cnt == 3) {
		Ans = Ans < sum ? sum : Ans;
		return;
	}
	else {
		for(int i=0; i<4; i++) {
			int nr = r + dr[i];
			int nc = c + dc[i];

			if(nr>=1 && nr<=N && nc>=1 && nc<=M) {
				if(VIT[nr][nc] == 0) {
					VIT[nr][nc] = 1;
					make(nr,nc,cnt+1,sum+MAP[nr][nc]);
					VIT[nr][nc] = 0;
				}
			}
		}
	}
}

void special(int r, int c) {
	for(int i=0; i<4; i++) {
		int sum = MAP[r][c];

		for(int j=0; j<3; j++) {
			int nr = r + dr[(i+j)%4];
			int nc = c + dc[(i+j)%4];

			if(nr>=1 && nc>=1 && nr<=N && nc<=M) sum += MAP[nr][nc];
		}

		Ans = Ans < sum ? sum : Ans;
	}
}

int main(int argc, char const *argv[])
{
	scanf("%d %d", &N, &M);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			scanf("%d", &MAP[r][c]);
		}
	}

	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			VIT[r][c] = 1;
			make(r,c,0,MAP[r][c]);
			VIT[r][c] = 0;
			special(r,c);
		}
	}

	printf("%d\n", Ans);
	return 0;
}