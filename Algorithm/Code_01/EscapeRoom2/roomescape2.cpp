#include <stdio.h>
#include <queue>

using namespace std;

int T, S, E;
int dr[2] = {-1, 1};
int V[50001];

queue < pair<int,int> > q;

int main(void) {
	scanf("%d",&T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d",&S,&E);

		for(int i=1; i<=50000; i++) {
			V[i] = 0;
		}

		while(!q.empty()) {
			q.pop();
		}

		q.push(make_pair(S,0));
		V[S] = 1;
		while(!q.empty()) {
			int r = q.front().first;
			int ans = q.front().second;
			printf("(%d %d)\n", r, ans);
			q.pop();

			if(r == E) {
				printf("#%d %d\n", test_case, ans);
				break;
			}

			if(r>=1 && r<=25000) {
				if(V[2*r]==0) {
					q.push(make_pair(2*r,ans+10));
					V[2*r] = 1;
				}
			}

			for(int i=0; i<2; i++) {
				int n_r = r + dr[i];
				int n_ans = ans + 10;

				if(n_r>=1 && n_r<=50000) {
					if(V[n_r]==0) {
						q.push(make_pair(n_r,n_ans));
						V[n_r] = 1;
					}
				}
			}
		}
	}
}