#include <stdio.h>
#include <algorithm>
#include <vector>
#define MAX 13

using namespace std;

int T, N;
long long minsum, maxsum;
vector < int > oper;
int num[MAX], visit[MAX];
int sz;

void dfs(long long sum, int numidx, int depth) {

	if(depth == N) {
		maxsum = max(maxsum, sum);
		minsum = min(minsum, sum);
		printf("maxsum : %lld, minsum : %lld\n", maxsum, minsum);
		return;
	}
	else {

		for(int i=0; i<sz; i++) {

			if(visit[i] != 1) {

				visit[i] = 1;

				if(oper[i] == 0) 
					sum += num[numidx];
				else if(oper[i] == 1)
					sum -= num[numidx];
				else if(oper[i] == 2)
					sum *= num[numidx];
				else
					sum /= num[numidx];

				dfs(sum,numidx+1,depth+1);

				visit[i] = 0;

				if(oper[i] == 0) 
					sum -= num[numidx];
				else if(oper[i] == 1)
					sum += num[numidx];
				else if(oper[i] == 2)
					sum /= num[numidx];
				else
					sum *= num[numidx];
			}
		}
	}
}

int main(void) {
	
	scanf("%d", &T);
	for(int tc=1; tc<=T; tc++) {

		scanf("%d", &N);

		while(!oper.empty())
			oper.pop_back();

		for(int i=0; i<4; i++) {

			int size;
			scanf("%d",&size);
			for(int j=0; j<size; j++) {

				oper.push_back(i); 				// 0 == +, 1 == -, 2 == *, 3 == /
			}
		}

		for(int i=0; i<N; i++) {

			scanf("%d",&num[i]);
			visit[i] = 0;
		}

		minsum = 2e13;
		maxsum = -2e13;
		sz = oper.size();
		dfs(num[0],1,1);

		printf("#%d %lld\n", tc, maxsum - minsum);
		
	}
}
