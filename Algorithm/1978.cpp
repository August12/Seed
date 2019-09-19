#include <stdio.h>

int N;
int target[101];
int answer;

void isPrime(int target) {
	bool flag = true;
	if(target == 1) return;

	for(int i=2; i<=1000; i++) {
		if(i != target && target % i == 0) {
			flag = false;
			break;
		}
	}
	if(flag) answer++;
}

int main(void) {
	scanf("%d", &N);
	for(int i=0; i<N; i++) {
		int target;
		scanf("%d",&target);
		isPrime(target);
	}
	printf("%d\n", answer);
}