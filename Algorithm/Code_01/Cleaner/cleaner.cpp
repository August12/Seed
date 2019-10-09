#include <stdio.h>

int T, N, A, B;
long long Answer;

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d %d", &N, &A, &B);

		for(int i=1; i<=N; i++) {
			int cls;
			scanf("%d", &cls);
			cls -= A;
			if(cls > 0) {
				if(cls%B == 0) {
					Answer += cls/B;
				}
				else {
					Answer += cls/B + 1;
				}
			}
		}
		printf("#%d %lld\n", test_case, Answer);
	}
}