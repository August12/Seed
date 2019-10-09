#include <stdio.h>
#include <stdlib.h>

using namespace std;

int N, M, H;
int Map[31][11];
int Answer;

int is_answer() {
	for(int c=1; c<=N; c++) {
		
		int tmp = c;
		for(int r=1; r<=H; r++) {

			if(Map[r][tmp] == 1) {
				tmp++;
			}
			else if(tmp-1 >= 1 && Map[r][tmp-1] == 1) {
				tmp--;
			}
		}

		if(tmp == c) {
			continue;
		}
		else {
			return 0;
		}
	}

	return 1;
}

void dfs(int row, int col, int depth, int limit) {
	if(depth == limit) {
		if(is_answer()) {
			Answer = limit;
			printf("%d\n", Answer);
			exit(0);
		}
	}
	else {
		for(int r=row; r<=H; r++) {
			for(int c=1; c<N; c++) {
				if(Map[r][c]==0) {
					Map[r][c] = 1;
					dfs(r,c,depth+1,limit);
					Map[r][c] = 0;
				}
			}
		}
	}
}

int main() {
	scanf("%d %d %d", &N, &M, &H);
	for(int line=1; line<=M; line++) {
		int y, x;
		scanf("%d %d", &y, &x);

		Map[y][x] = 1;
	}

	Answer = -1;
	if(is_answer()) {
		Answer = 0;
		printf("%d\n", Answer);
	}
	else {
		for(int plus=1; plus<=3; plus++) {

			dfs(1,1,0,plus);
		}

		printf("%d\n", Answer);
	}
}