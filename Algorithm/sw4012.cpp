#include <stdio.h>
#include <vector>

using namespace std;

int T, N;
int S[17][17];
int check[17];
long long total;
int answer;
vector <int> a;
vector <int> b;

void sol(void) {
	int A=0, B=0;

	for(int i=1; i<=N; i++) {
		if(check[i] == 0) {
			b.push_back(i);
		}
	}

	for(int i=0; i<a.size(); i++) {
		for(int j=i+1; j<a.size(); j++) {
			int sa = a[i];
			int sb = a[j];
			int sc = b[i];
			int sd = b[j];

			A += S[sa][sb];
			A += S[sb][sa];
			B += S[sc][sd];
			B += S[sd][sc];
		}
	}

	int diff = 0;
	if(A < B) {
		diff = B-A;
	}
	else {
		diff = A-B;
	}

	if(answer > diff) {
		answer = diff;
	}
	b.clear();
	return;
}

void dfs(int target, int depth) {
	if(depth > N/2) {
		// for(int i=0; i<N/2; i++) {
		// 	printf("%d ", a[i]);
		// }
		// printf("\n");
		sol();
		return;
	}
	else {
		for(int i=target; i<=N; i++) {
			a.push_back(i);
			check[i] = 1;
			dfs(i+1, depth+1);
			check[i] = 0;
			a.pop_back();
		}
	}
}

int main(void) {
	freopen("sample_input.txt","r",stdin);

	scanf("%d",&T);
	for(int tc=1; tc<=T; tc++) {
		scanf("%d",&N);
		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				scanf("%d",&S[y][x]);
			}
		}
		memset(check,0,sizeof(check));
		a.clear();
		answer = 2e9;

		dfs(1,1);
		printf("#%d %d\n", tc, answer);
	}
}
