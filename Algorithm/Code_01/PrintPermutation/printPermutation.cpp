#include <stdio.h>

int T, N;
int visited[7];
int arr[7];

void DFS(int depth) {
	if(depth == N+1) {
		for (int i=1; i<=N; i++) {
			printf("%d ", arr[i]);
		}
		printf("\n");
	}
	else {
		for (int i=1; i<=N; i++) {
			if(visited[i]==0) {
				visited[i] = 1;
				arr[depth] = i;
				DFS(depth+1);
				visited[i] = 0;
				arr[depth] = 0;
			}
		}
	}
}

int main(void) {
	scanf("%d",&T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d",&N);
		for(int i=1; i<=N; i++) {
			visited[i] = 0;
		}
		printf("#%d\n", test_case);
		DFS(1);
	}
}