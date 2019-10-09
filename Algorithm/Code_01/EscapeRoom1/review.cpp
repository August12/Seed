/* 19.2.21 방탈출 게임 */
#include <stdio.h>

int T;
int N, M;
int P[1001];
int Answer;

void dfs(int room, int sum) {
	if(room == M) {
		if(sum > Answer) {
			Answer = sum;
		}
	}
	else {
		if(room <= M) {
			dfs(P[room],sum+1);
		}
	}
}

int main(void) {
	scanf("%d", &T);
	for(int test_case=1;test_case<=T;test_case++) {
		scanf("%d %d", &N, &M);
		for(int room_num=1;room_num<N;room_num++) {
			scanf("%d", &P[room_num]);
		}

		Answer = -1;

		for(int room_num=1;room_num<M;room_num++) {
			dfs(room_num,0);
		}
		printf("#%d %d\n", test_case, Answer);
	}
}

