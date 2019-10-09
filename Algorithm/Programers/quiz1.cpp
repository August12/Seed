#include <stdio.h>
#include <stdlib.h>

bool solution(int A[], int B[], int lenA, int lenB) {
	int count = 0;

	if(lenA!=lenB) return false;

	for(int i=0;i<lenB;i++) {
		if(count > lenB) break;
		if(A[i]==B[i])
			continue;
		else {
			int tmp = A[lenA-1];
			for(int j=lenA-1;j>0;j--) {
				A[j] = A[j-1];
			}
			A[0] = tmp;
			i=0;
			count++;
		}
	}
	if(count > lenA) return false;
	else return true;
}

int main() {
	int arrA[] = {1,2,3,4};
	int arrB[] = {3,4,2,1};

	int arrC[] = {1,2,3,4,5,6};
	int arrD[] = {1,2,3,4};

	int arrE[] = {7,8,10};
	int arrF[] = {10,7,8};

	int arrG[] = {4,3,2,1};
	int arrH[] = {5,4,1,2};

	int arrI[] = {1,2,3,4,5,6,7};
	int arrJ[] = {2,3,4,5,6,7,1};

	printf("%s\n", solution(arrA,arrB,sizeof(arrA)/sizeof(int),sizeof(arrB)/sizeof(int)) ? "true" : "false"); 
	printf("%s\n", solution(arrC,arrD,sizeof(arrC)/sizeof(int),sizeof(arrD)/sizeof(int)) ? "true" : "false");
	printf("%s\n", solution(arrE,arrF,sizeof(arrE)/sizeof(int),sizeof(arrF)/sizeof(int)) ? "true" : "false");
	printf("%s\n", solution(arrG,arrH,sizeof(arrG)/sizeof(int),sizeof(arrH)/sizeof(int)) ? "true" : "false");
	printf("%s\n", solution(arrI,arrJ,sizeof(arrI)/sizeof(int),sizeof(arrJ)/sizeof(int)) ? "true" : "false");

	return 0;
}