#include <stdio.h>
#include <algorithm>

using namespace std;

struct st {
	int t1, t2;
};

//t1->t2순으로 오름차순 정렬
bool comp1(const struct st &a, const struct st &b) {
	if(a.t1 == b.t1) return a.t2 < b.t2;
	else return a.t1 < b.t1;
}

int main() {
	st arr[10];
	for(int i=0; i<10; i++) {
		scanf("%d %d", &arr[i].t1, &arr[i].t2);
	}

	for(int i=0; i<10; i++) {
		printf("arr[%d]:%d %d\n", i, arr[i].t1, arr[i].t2);
	}

	sort(arr+0, arr+10, comp1);
	for(int i=0; i<10; i++) {
		printf("arr[%d]:%d %d\n", i, arr[i].t1, arr[i].t2);
	}
}