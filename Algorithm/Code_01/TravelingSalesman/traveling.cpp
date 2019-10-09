#include <stdio.h>

int T, N, M;
int Map[11][11];
int Visit[11];
int Ans = 501;
int count;

void dfs(int city, int sum) {
	if(count == N-1) {
		if(Map[city][M] != 0) {
			Ans = (Ans >= sum+Map[city][M]) ? sum+Map[city][M] : Ans;
		}
	}

	for(int i=1; i<=N; i++) {
		if(Map[city][i] != 0 && Visit[i] == 0) {
			if(Ans > sum + Map[city][i]) {
				Visit[i] = 1;
				count++;
				dfs(i,sum + Map[city][i]);
				Visit[i] = 0;
				count--;
			}
		}
	}
}


int main(void) {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d", &N, &M);

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				Map[i][j] = 0;
			}
		}

		for(int i=1; i<=N; i++) {
			Visit[i] = 0;
		}

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				scanf("%d", &Map[i][j]);
			}
		}
		Ans = 501;
		count = 0;

		Visit[M] = 1;
		dfs(M,0);
		printf("#%d %d\n", test_case,Ans);
	}
}
