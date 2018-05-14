#include <stdio.h>

int L;

int main() {

	scanf("%d",&L);

	if(L%5 == 0)
		printf("%d\n", L/5);
	else if(L<5)
		printf("%d\n", 1);
	else
		printf("%d\n", (L/5)+1);

	return 0;

}