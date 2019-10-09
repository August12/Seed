/*  19.2.19
/	<문제>
/	순열을 출력한다. 2초이내, 256MB이내.
/	1부터 N(<=6)까지 순열을 앞 수를 우선하여 오름차순 순서대로 출력.
/	입력은 테스트케이스와 각 케이스마다 N값이 주어짐.
/	ex) N=3이면 1 2 3, 1 3 2, 2 1 3, 2 3 1, 3 1 2, 3 2 1. 출력.
/
/	<생각>
/	깊이 우선 탐색으로 접근 해보자. 먼저 워스트케이스를 생각해본다.
/	N=6이면 6!의 가지 수가 생성된다.720번 연산이 진행? 2초안에 가능. */

#include <stdio.h>

int T, N;
int visited[7];
int answer[7];

void dfs(int depth) {
	if(depth == N+1) {
		for(int i=1; i<=N; i++) {
			printf("%d ", answer[i]);
		}
		printf("\n");
	}
	else {
		for(int i=1; i<=N; i++) {
			if(visited[i] == 0) {
				visited[i] = 1;
				answer[depth] = i;
				dfs(depth+1);
				visited[i] = 0;
				answer[depth] = 0;
			}
		}
	}
}

int main(void) {
	scanf("%d",&T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d",&N);
		printf("#%d\n", test_case);
		dfs(1);
	}
	return 0;
}
