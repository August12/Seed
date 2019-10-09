#include <stdio.h>

int T;
int N, K, P;
int Y, H;
int A[101];
int DAY;

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		int tmp = 0;
		DAY = -1;
		Y = 0;
		H = 0;
		for(int i=0; i<=100; i++) {
			A[i] = 0;
		}

		scanf("%d %d %d", &N, &K, &P);
		for(int i=1; i<=N; i++) {
			scanf("%d", &A[i]);
			tmp = tmp + A[i];
		}

		if(K > tmp) {
			printf("#%d %d\n", test_case, DAY);
		}
		else {
			DAY++;
			for(int i=1; i<=N; i++) {
				Y = Y + A[i];
				if(Y >= P) {
					Y -= P;
					H += P;
					DAY++;
				}
				else {
					H += Y;
					Y = 0;
					DAY++;
				}

				if(H >= K) {
					break;
				}
			}

			if(H >= K) {
				printf("#%d %d\n", test_case, DAY);
			}
			else {
				DAY = -1;
				printf("#%d %d\n", test_case, DAY);
			}
		}

	}
	return 0;

}