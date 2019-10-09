/* 난수 생성 코드 cpp */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	freopen("Input_RobotRat.txt", "w", stdout);
	int T = rand()%50 + 1; // % 범위 수, + 시작하는 수, <rand 0~32767>
	printf("%d\n", T);

	for(int test_case=0;test_case<T;test_case++) {
		int N = rand()%50 + 3;
		int M = rand()%50 + 3;
		printf("%d %d\n", N, M);

		int R = rand()%(N-1)+1;
		int C = rand()%(M-1)+1;
		int D = rand()%4;
		printf("%d %d %d\n", R, C, D);

		for(int i=0;i<N;i++) {
			for(int j=0;j<M;j++) {
				if(i==0 || i==N-1) printf("1 ");
				else if(j==0 || j==M-1) printf("1 ");
				else if(i==R-1 && j==C-1) printf("0 ");
				else printf("%d ", rand()%2);
			}
			printf("\n");
		}
	}
}