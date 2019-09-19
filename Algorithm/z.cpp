#include <stdio.h>
#include <algorithm>
#include <vector>
#define MAX 13

using namespace std;

int T, N;
int minsum, maxsum;
vector < int > oper;
int num[MAX], visit[MAX];
int sz;
/*
void dfs(int sum, int numidx, int depth) {

	if(depth == N) {
		maxsum = max(maxsum, sum);
		minsum = min(minsum, sum);
		printf("\n");
		printf("maxsum : %d, minsum : %d\n", maxsum, minsum);
		return;
	}
	else {

		for(int i=0; i<sz; i++) {

			if(visit[i] != 1) {

				visit[i] = 1;

				if(oper[i] == 0) {
					sum += num[numidx];
					printf("+ %d", num[numidx]);
				}
				else if(oper[i] == 1){
					sum -= num[numidx];
					printf("- %d", num[numidx]);
				}
				else if(oper[i] == 2){
					sum *= num[numidx];
					printf("* %d", num[numidx]);
				}
				else{
					sum /= num[numidx];
					printf("/ %d", num[numidx]);
				}

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
*/
int main(void) {
	
	scanf("%d", &T);
	/*
	for(int tc=1; tc<=T; tc++) {

		scanf("%d", &N);

		while(!oper.empty())
			oper.pop_back();

		for(int i=0; i<4; i++) {

			int size;
			scanf("%d",&size);
			for(int j=size; j>0; j--) {

				oper.push_back(i); 				// 0 == +, 1 == -, 2 == *, 3 == /
			}
		}

		for(int i=0; i<N; i++) {

			scanf("%d",&num[i]);
			visit[i] = 0;
		}

		minsum = 1e9;
		maxsum = -1e9;
		sz = oper.size();
		dfs(num[0],1,1);

		printf("#%d %d\n", tc, maxsum - minsum);
	
	}*/
}