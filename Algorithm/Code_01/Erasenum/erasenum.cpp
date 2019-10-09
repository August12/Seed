#include <stdio.h>
#include <algorithm>

using namespace std;

int T, N;
int A[100001];
long long DP[100001][2];
long long Answer;

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d", &N);
		for(int i=1; i<=N; i++) {
			scanf("%d", &A[i]);
		}
		Answer = 0;

		DP[1][0] = A[1];
		DP[1][1] = 0;
		for(int i=2; i<=N; i++) {
			if(i == 2) {
				DP[i][0] = A[2];
				DP[i][1] = DP[1][0];
			}
			else {
				DP[i][0] = max(DP[i-2][0],DP[i-2][1]) + A[i];
				DP[i][1] = DP[i-1][0];
			}
		}

		if(N == 1) {
			Answer = DP[1][0];
		}
		else {
			Answer = max(DP[N][0],DP[N][1]);
		}

		printf("#%d %lld\n", test_case, Answer);
	}
}