#include <stdio.h>
#include <stdlib.h>

int main() {
	int T;

	freopen("random_input.txt","r",stdin);
	scanf("%d",&T);

	for(int test_case=0;test_case<T;test_case++) {
		int A=0,B=0;
		int day=0;
		scanf("%d",&A);
		scanf("%d",&B);

		while(A>=B) {
			A *= 2;
			B *= 3;
			day++;
		}

		printf("#%d %d\n", test_case+1, day);
	}
}