#include <stdio.h>

int main() {
	int T;
	int N,M;

	scanf("%d",&T);
	for(int test_case=1; test_case<=T; test_case++) {

		int GRAPH[101][101] = {0,}; 
		scanf("%d %d",&N,&M);
		for(int i=0; i<M; i++) {
			int v1,v2;
			scanf("%d %d",&v1,&v2);
			GRAPH[v1][v2] = 1;
		}

		printf("#%d\n", test_case);
		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				printf("%d ", GRAPH[i][j]);
			}
			printf("\n");
		}

		for(int row=1; row<=N; row++) {
			int tmp[101] = {0,};
			int len = 0;
			for(int col=1; col<=N; col++) {
				if(GRAPH[row][col]==1) {
					tmp[len] = col;
					len++;
				}
			}

			for(int vertex=0; vertex<len; vertex++) {
				printf("%d ", tmp[vertex]);
			}
			printf("\n");
		}
	}


	return 0;
}