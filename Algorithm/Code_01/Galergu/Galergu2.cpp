#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
	int T;

	freopen("random_input.txt","r",stdin);
	scanf("%d",&T);

	for(int test_case=0;test_case<T;test_case++) {
		double A=0,B=0;
		double day=0;
		scanf("%lf",&A);
		scanf("%lf",&B);

		day = int((log(A/B)) / (log(3/2)));

		printf("%f\n", log(A));
		printf("%f\n", log(B));
		printf("%f\n", day);

		printf("%f\n", log(3/2));

		printf("#%d %f\n", test_case+1, day);
	}
}