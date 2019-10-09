/*
	와인을 구매하는 최소 비용으로 구매해야한다.
	한 번 구매한 와인은 다시 입고되지 않는다.
	먼저 N병을 사야하고 다음날 M병을 사야한다.
	첫날 금액 - 다음날 금액으로 정렬한뒤 구매하면 최소비용으로 살 수 있다.
*/

#include <stdio.h>
#include <algorithm>

using namespace std;

typedef struct wine {
	int p1;
	int p2;
	int diff;
} Wine;

int T;
int N, M;
Wine W[100001];
int Answer;

bool comp(const Wine &a, const Wine &b) {
	return a.diff < b.diff;
}

int main() {
	scanf("%d",&T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d",&N,&M);
		for(int i=1; i<=N+M; i++) {
			scanf("%d",&W[i].p1);
		}
		for(int i=1; i<=N+M; i++) {
			scanf("%d",&W[i].p2);
		}
		for(int i=1; i<=N+M; i++) {
			W[i].diff = W[i].p1 - W[i].p2;
		}

		sort(W+1, W+N+M+1, comp);
		Answer = 0;
		for(int i=1; i<=N; i++) {
			Answer += W[i].p1;
		}
		for(int i=N+1; i<=M+N; i++) {
			Answer += W[i].p2;
		}

		printf("#%d %d\n", test_case, Answer);
	}
}