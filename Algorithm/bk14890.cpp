#include <stdio.h>
#include <algorithm>

int N, L;
int Map[101][101];
int Vit[101][101];
int Answer;

int is_right_make(int r, int c) {
	if(Map[r][c] - Map[r][c+1] > 0) {
		if(c+L > N) return 0;

		int land = c+1;
		for(int i=0; i<L; i++) {
			if(Map[r][land] != Map[r][land + i]) return 0;
			if(Vit[r][land + i] == 1) return 0;
		}

		for(int i=0; i<L; i++) {
			Vit[r][land + i] = 1;
		}
		return 1;
	}
	else {
		if(c-L+1 < 1) return 0;

		int land = c;
		for(int i=0; i<L; i++) {
			if(Map[r][land] != Map[r][land-i]) return 0;
			if(Vit[r][land-i] == 1) return 0;
		}

		for(int i=0; i<L; i++) {
			Vit[r][land-i] = 1;
		}
		return 1;
	}
}

int is_down_make(int r, int c) {
	if(Map[r][c] - Map[r+1][c] > 0) {
		if(r+L > N) return 0;

		int land = r+1;
		for(int i=0; i<L; i++) {
			if(Map[land][c] != Map[land + i][c]) return 0;
			if(Vit[land + i][c] == 1) return 0;
		}

		for(int i=0; i<L; i++) {
			Vit[land + i][c] = 1;
		}
		return 1;
	}
	else {
		if(r-L+1 < 1) return 0;

		int land = r;
		for(int i=0; i<L; i++) {
			if(Map[land][c] != Map[land-i][c]) return 0;
			if(Vit[land-i][c] == 1) return 0;
		}

		for(int i=0; i<L; i++) {
			Vit[land-i][c] = 1;
		}
		return 1;
	}
}

int main() {
	scanf("%d %d", &N, &L);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=N; c++) {
			scanf("%d", &Map[r][c]);
		}
	}

	Answer = 2*N;
	for(int r=1; r<=N; r++) {
		for(int c=1; c<N; c++) {
			if(Map[r][c] == Map[r][c+1]) continue;
			else if(abs(Map[r][c] - Map[r][c+1]) == 1) {
				if(is_right_make(r,c) == 1) {
					if(Map[r][c] - Map[r][c+1] > 0) {
						c = c + L - 1;
						continue;
					}
					else continue;
				}
				else {
					Answer--;
					break;
				}
			}
			else {
				Answer--;
				break;
			}
		}
	}

	for(int r=1; r<=N; r++) {
		for(int c=1; c<=N; c++) {
			Vit[r][c] = 0;
		}
	}

	for(int c=1; c<=N; c++) {
		for(int r=1; r<N; r++) {
			if(Map[r][c] == Map[r+1][c]) continue;
			else if(abs(Map[r][c] - Map[r+1][c]) == 1) {
				if(is_down_make(r,c) == 1) {
					if(Map[r][c] - Map[r+1][c] > 0) {
						r = r + L - 1;
						continue;
					}
					else continue;

				}
				else {
					Answer --;
					break;
				}
			}
			else {
				Answer--;
				break;
			}
		}
	}

	printf("%d\n", Answer);
}