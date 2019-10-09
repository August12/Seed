#include <stdio.h>
#include <algorithm>

using namespace std;

int arr[100];
int arr2[100];
int main() {
	arr[0] = 5;
	arr[1] = 3;
	arr[2] = 1;
	arr2[0] = 45;
	arr2[1] = 2;
	arr2[2] = 33;
	arr2[3] = 100;
	arr2[4] = 9;
	arr2[5] = 21; 
	for(int i=0; i<6; i++) {
		printf("%d ", arr2[i]);
	}
	printf("\n");

	//ascending order
	sort(arr2+0,arr2+6);
	for(int i=0; i<6; i++) {
		printf("%d ", arr2[i]);
	}
	printf("\n");

	//descending order 1
	// for(int i=0; i<6; i++) {
	// 	printf("%d ", arr2[2-i]);
	// }
	// printf("\n");

	//descending order 2
	for(int i=0; i<6; i++) {
		arr2[i] = arr2[i] * (-1);
	}
	sort(arr2+0,arr2+6);
	for(int i=0; i<6; i++) {
		arr2[i] = arr2[i] * (-1);
	}
	sort(arr+0,arr+6);
	for(int i=0; i<6; i++) {
		printf("%d ", arr2[i]);
	}
	printf("\n");
}