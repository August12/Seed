#include <stdio.h>
#include <string.h>
#include <math.h>

int N;

bool isPrime(int target) {
	bool ret = true;
	int candidate[1000001];
	memset(candidate,0,sizeof(candidate));

	for(int i=2; i<=sqrt(target); i++) {
		if(candidate[i] == 1) continue;

		for(int j=i; j<=sqrt(target); j+=i) {
			
			if(j != target && target % j == 0) {
				ret = false;
				break;
			}
			candidate[j] = 1;
		}
	}
	return ret;
}

int main(void) {
	while(true) {
		int target = 0;
		scanf("%d",&target);
		if(target == 0) break;

		int minPrime = 0;
		int maxPrime = 0;

		for(int i = 2; i <= 1000000; i++) {
			if(isPrime(i)) {
				int big_num = target - i;

				if(isPrime(big_num)) {
					maxPrime = big_num;
					minPrime = i;
					break;
				}
			}
		}

		if(minPrime == 0) printf("Goldbach's conjecture is wrong.\n");
		else printf("%d = %d + %d\n", target, minPrime, maxPrime);
	}
}