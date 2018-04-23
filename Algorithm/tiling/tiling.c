#include <stdio.h>

int d[1001] = { 0 };

int dp(int x) {

	if(x == 1)
		return 1;
	if(x == 2)
		return 2;
	
	if(d[x] != 0)
		return d[x];
	else{
		return d[x] = (dp(x-1) + dp(x-2))%10007;
	}
}

int main(void) {
	int x;

	scanf("%d",&x);
	printf("%d\n",dp(x));

}