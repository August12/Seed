#include <stdio.h>

using namespace std;

int N, K;
int arr[201];

typedef struct {
	int fre_color;
	int start;
	int last;
}COL_INF;

bool checkArray() {
	for(int i=0;i<N-1;i++) {
		if(arr[i]!=arr[i+1])
			return false;
	}
	return true;
}

COL_INF findColor() {
	COL_INF inform;
	int max = 0, count = 0;

	for(int i=0;i<N-1;i++) {
		count++;
		if(arr[i]!=arr[i+1]) {
			if(max<count) {
				max = count;
				inform.fre_color = arr[i];
				inform.last = i;
			}
			count=0;
		}
	}
	inform.start = inform.last - max + 1;

	return inform;
}

int change(COL_INF inform) {
	int answer = 0;
	int s = inform.start;
	int l = inform.last;
	int c;

	if(s-1 >= 0 && l+1 < N) {
		if(arr[s-1] == arr[l+1]) {
			c = arr[s-1];
			for(int i=0;i<l-s+1;i++) {
				arr[i] = c;
			}
		}
	}
}

int main() {
	scanf("%d %d",&N,&K);

	for(int i=0;i<N;i++) {
		scanf("%d",&arr[i]);
	}


	while(!checkArray()) {
		COL_INF = cur_inform;
		cur_inform = findColor();


		return 0;

	}

}