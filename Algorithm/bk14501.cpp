#include <stdio.h>

int N;
int PLAN[16][2];
int Ans;

void profit(int pos, int sum) {
	
	if(pos > N) {
		Ans = Ans < sum ? sum : Ans;
		return;
	}
	else {
		if(PLAN[pos][0] + pos - 1 <= N) {
			profit(pos+PLAN[pos][0],sum+PLAN[pos][1]);
		}
		if(PLAN[pos][0] + 1 <= N) {
			profit(pos+1,sum);
		}
	}
}

int main() {
	scanf("%d", &N);
	for(int i=1; i<=N; i++) {
		scanf("%d %d", &PLAN[i][0], &PLAN[i][1]);
	}

	profit(1,0);
	printf("%d\n", Ans);

}