#include <stdio.h>

int R, C, T;
int MAP[51][51], CPY[51][51];
int dr[4] = {0, -1, 0, 1};
int dc[4] = {1, 0, -1, 0};
int mu_r, mu_c, md_r, md_c, cur, pre;
int answer;

void copy_map(void) {
	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			CPY[r][c] = MAP[r][c];
		}
	}
}

void map_copy(void) {
	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			MAP[r][c] = CPY[r][c];
		}
	}
}

void spread(void) {
	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			if(MAP[r][c] != 0) {
				int cnt = 0;
				int dust = MAP[r][c]/5;
				for(int i=0; i<4; i++) {
					int nr = r + dr[i];
					int nc = c + dc[i];
					if(nr >= 1 && nr <= R && nc >= 1 && nc <= C) {
						if(MAP[nr][nc] != -1) {
							cnt++;
							CPY[nr][nc] += dust;
						}
					}
				}
				CPY[r][c] -= dust*cnt;
			}
		}
	}
}

void right(int r, int c) {
	int nc = c+1;

	cur = MAP[r][c];
	MAP[r][c] = pre;
	pre = cur;

	if(nc <= C) right(r, nc);
}

void up(int r, int c) {
	int nr = r-1;

	cur = MAP[r][c];
	MAP[r][c] = pre;
	pre = cur;

	if(nr == md_r && c == md_c) return;
	if(nr >= 1) up(nr, c);	
}

void down(int r, int c) {
	int nr = r+1;

	cur = MAP[r][c];
	MAP[r][c] = pre;
	pre = cur;

	if(nr == mu_r && c == mu_c) return;
	if(nr <= R) down(nr, c);
}

void left(int r, int c) {
	int nc = c-1;

	cur = MAP[r][c];
	MAP[r][c] = pre;
	pre = cur;

	if(nc >= 1) left(r, nc);
}

void print_MAP(void) {
	printf("#MAP : \n");
	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			printf("%d ", MAP[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(void) {
	int flag = 1;

	scanf("%d %d %d", &R, &C, &T);
	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			scanf("%d", &MAP[r][c]);

			if(MAP[r][c] == -1) {
				if(flag) {
					mu_r = r;
					mu_c = c;
					flag = 0;
				}
				else {
					md_r = r;
					md_c = c;
				}
			}

		}
	}
	copy_map();

	for(int time=1; time<=T; time++) {
		//printf("#time : %d\n", time);
		spread();
		map_copy();
		//print_MAP();
		
		pre = 0;
		cur = 0;
		right(mu_r,mu_c+1);
		up(mu_r-1,C);
		left(1,C-1);
		down(2,1);
		//print_MAP();

		pre = 0;
		cur = 0;
		right(md_r,md_c+1);
		down(md_r+1,C);
		left(R,C-1);
		up(R-1,1);
		copy_map();
		//print_MAP();
	}

	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			if(MAP[r][c] == -1)	continue;
			answer += MAP[r][c];
		}
	}

	printf("%d\n", answer);
}
























