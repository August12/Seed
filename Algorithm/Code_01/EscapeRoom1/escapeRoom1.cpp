#include <stdio.h>

int T, N, M;
int potal[1001];
int answer, tmp;

int dfs(int room) {
	if(room == M) {
		return tmp;
	}
	else {
		if(room != N) {
			tmp++;
			return dfs(potal[room]);
		}
		else {
			return -1;
		}
	}
}

int main(void) {
	answer = -1;
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d", &N, &M);
		for(int room=1; room<N; room++) {
			scanf("%d", &potal[room]);
		}

		for(int room=1; room<=N; room++) {
			if(room == M) continue;
			else {
				int tmp2 = dfs(room);
				answer = (tmp2>answer) ? tmp2 : answer;
				tmp = 0;
			}
		}
		printf("#%d %d\n", test_case, answer);
		answer = -1;
	}
}

