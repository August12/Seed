#include <stdio.h>
using namespace std;
int T;
int N,W;
int inside[10001],outside[10001];

/*
1
2 100
1 2
3 4
*/

int main() {
	scanf("%d",&T);

	for(int testcase=0;testcase<T;testcase++) {
		int answer=0;
		int inoccp[10001]={0,},outoccp[10001]={0,};

		scanf("%d %d",&N,&W);
		for(int j=0;j<N;j++) {
			scanf("%d",&inside[j]);
		}
		for(int j=0;j<N;j++) {
			scanf("%d",&outside[j]);
		}

		if(N==1) {
			if(inside[0]+outside[0]<=W) {
				answer++;
				printf("%d\n", answer);
				return 0;
			}
			else {
				answer = 2;
				printf("%d\n", answer);
				return 0;
			}
		}

		for(int i=0;i<N;i++) {
			int left = (i+(N-1))%N; 
			int right = (i+1)%N;

			if(inoccp[left] == 0 && inoccp[i] == 0) {
				if(inside[left]+inside[i]<=W) {
					inoccp[left] = 1;
					inoccp[i] = 1;
					answer++;
				}
			}

			if(inoccp[right] == 0 && inoccp[i] == 0) {
				if(inside[right]+inside[i]<=W) {
					inoccp[right] = 1;
					inoccp[i] = 1;
					answer++;
				}
			}

			if(outoccp[i] == 0 && inoccp[i] == 0) {
				if(outside[i]+inside[i]<=W) {
					outoccp[i] = 1;
					inoccp[i] = 1;
					answer++;
				}
			}
		}

		for(int i=0;i<N;i++) {
			int left = (i+(N-1))%N; 
			int right = (i+1)%N;

			if(outoccp[left] == 0 && outoccp[i] == 0) {
				if(outside[left]+outside[i]<=W) {
					outoccp[left] = 1;
					outoccp[i] = 1;
					answer++;
				}
			}

			if(outoccp[right] == 0 && outoccp[i] == 0) {
				if(outside[right]+outside[i]<=W) {
					outoccp[right] = 1;
					outoccp[i] = 1;
					answer++;
				}
			}

			if(outoccp[i] == 0 && inoccp[i] == 0) {
				if(outside[i]+inside[i]<=W) {
					outoccp[i] = 1;
					inoccp[i] = 1;
					answer++;
				}
			}
		}

		for(int i=0;i<N;i++) {
			if(inoccp[i] == 0) answer++;
		}
		for(int i=0;i<N;i++) {
			if(outoccp[i] == 0) answer++;
		}

		printf("%d\n", answer);
	}
}