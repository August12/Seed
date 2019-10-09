#include <stdio.h>
#include <algorithm>

using namespace std;

int T;
int N;
int Score[10001];
int dp[10001][2];

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d", &N);
		for(int i=1; i<=N; i++) {
			scanf("%d", &Score[i]);
		}

		dp[1][0] = Score[1];
		dp[2][0] = Score[2];
		dp[2][1] = Score[2] + dp[1][0];
		dp[3][0] = Score[3] + dp[1][0];
		dp[3][1] = Score[3] + dp[2][0];
		for(int i=4; i<=N; i++) {
			dp[i][0] = max(Score[i] + dp[i-2][0], Score[i] + dp[i-2][1]);
			dp[i][1] = Score[i] + dp[i-1][0];
		}

		printf("#%d %d\n", test_case, max(dp[N][0],dp[N][1]));
	}
}