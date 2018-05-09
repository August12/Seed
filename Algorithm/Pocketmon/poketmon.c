#include <stdio.h>
#include <time.h>
int N;
int K;
int i = 0,j = 0;
int mul[1000000];


int main() {

	int test_case;
	int t,r = 0;
	int k;
	int max;
	int temp;
	scanf("%d",&test_case);

	for(t=0;t<test_case;t++) {

		scanf("%d",&N);
		scanf("%d",&K);

		for(i=0;i<=N;i++) {
			for(j=0;j<=N;j++) {
				mul[r] = i*j;
				printf("%d", mul[r]);
				r++;
			}
			printf("\n");
		}

		
		k = r;
		for(i=0;i<k;i++) {
			for(j=0;j<k-1;j++) {
				if(mul[j] > mul[j+1]) {
					max = mul[j];
					mul[j] = mul[j+1];
					mul[j+1] = max;
				}
			}
		}

		i = 1;
		max = mul[r-1];
		while(i<K) {
			if(max == mul[r-2]) {
				printf("%d == %d\n", max, mul[r-2]);
				r--;
			}
			else {
				printf("%d < %d\n", max, mul[r-2]);
				i++;
				printf("i:%d - K:%d\n", i, K);
				max = mul[r-2];
				printf("max:%d\n", max);
				r--;
			}

		}
		printf("%d\n", max);
	}
}