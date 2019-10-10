#include <stdio.h>
#include <string.h>

using namespace std;

int T, N;
int map[21][21];
int vit[21][21];
int answer = -1;
int num[101];
int sy, sx;

void rightup(int y, int x, int len) {
	//printf("ru(%d %d)\n", y, x);
	if(y-1>=1 && x+1<=N) {
		if(y-1 == sy && x+1 == sx) {
			if(len < 4) return;
			if(answer <= len) {
				answer = len;
				return;
			}
		}
		else {
			if(num[map[y-1][x+1]] == 0 && vit[y-1][x+1] == 0) {
				num[map[y-1][x+1]] = 1;
				vit[y-1][x+1] = 1;
				rightup(y-1,x+1,len+1);
				vit[y-1][x+1] = 0;
				num[map[y-1][x+1]] = 0;
			}
		}
	}
}

void leftup(int y, int x, int len) {
	//printf("lu(%d %d)\n", y, x);
	if(y-1>=1 && x-1>=1) {
		if(num[map[y-1][x-1]] == 0 && vit[y-1][x-1] == 0) {
			num[map[y-1][x-1]] = 1;
			vit[y-1][x-1] = 1;
			leftup(y-1, x-1, len+1);
			num[map[y-1][x-1]] = 0;
			vit[y-1][x-1] = 0;
			rightup(y,x,len);
		}
		else {
			rightup(y, x, len);
			return;
		}
	}
	else {
		rightup(y, x, len);
		return;
	}
}

void leftdown(int y, int x, int len) {
	//printf("ld(%d %d)\n", y, x);
	if(y+1<=N && x-1>=1) {
		if(num[map[y+1][x-1]] == 0 && vit[y+1][x-1] == 0) {
			num[map[y+1][x-1]] = 1;
			vit[y+1][x-1] = 1;
			leftdown(y+1, x-1, len+1);
			num[map[y+1][x-1]] = 0;
			vit[y+1][x-1] = 0;
			leftup(y,x,len);
		}
		else {
			leftup(y,x,len);
			return;
		}
	}
	else {
		leftup(y,x,len);
		return;
	}
}

void rightdown(int y, int x, int len) {
	//printf("rd(%d %d)\n", y, x);
	if(y+1<=N && x+1<=N) {
		if(num[map[y+1][x+1]] == 0 && vit[y+1][x+1] == 0) {
			num[map[y+1][x+1]] = 1;
			vit[y+1][x+1] = 1;
			rightdown(y+1, x+1, len+1);
			num[map[y+1][x+1]] = 0;
			vit[y+1][x+1] = 0;
			leftdown(y,x,len);
		}
		else {
			leftdown(y,x,len);
		}
	}
	else {
		leftdown(y,x,len);
	}

}

// void findDesert(int y, int left, int right, int len, int sum) {
// 	if(y+1<=N && left-1>0 && right+1<=N) {
// 		printf("(%d %d %d %d %d)\n", y, left, right, len, sum);

// 		if(num[map[y+1][left-1]] == 0) {
// 			num[map[y+1][left-1]] = 1;
// 			sum += map[y+1][left-1];

// 			if(num[map[y+1][right+1]] == 0) {
// 				num[map[y+1][right+1]] = 1;
// 				sum += map[y+1][right+1];
// 				findDesert(y+1, left-1, right+1, len+1, sum);
// 				num[map[y+1][right+1]] = 0;
// 				sum -= map[y+1][right+1];
// 			}

// 			num[map[y+1][left-1]] = 0;
// 			sum -= map[y+1][left-1];
// 		}
// 	}

// 	if(len == 0) return;
// 	else {
// 		for(int i=len; i>0; i--) {
// 			if(num[map[y+1][left+1]] == 0) {
// 				num[map[y+1][left+1]] = 1;
// 				sum += map[y+1][left+1];

// 				printf("(%d %d %d %d)\n", y+1, left+1, len, sum);

// 				if(len == 1) break;

// 				if(num[map[y+1][right-1]] == 0) {
// 					num[map[y+1][right-1]] = 1;
// 					sum += map[y+1][right-1];
// 				}
// 				else
// 					return;
// 			}
// 			else
// 				return;
// 		}

// 		if(answer <= sum) {
// 			answer = sum;
// 		}
// 	}
// }

int main(void) {
	freopen("sample_input.txt","r",stdin);

	scanf("%d",&T);
	for(int tc=1; tc<=T; tc++) {
		
		answer = -1;
		scanf("%d",&N);
		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				scanf("%d",&map[y][x]);
			}
		}

		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				memset(num, 0, sizeof(num));
				sy = y;
				sx = x;
				num[map[y][x]] = 1;
				vit[y][x] = 1;
				rightdown(y,x,1);
				vit[y][x] = 0;
			}
		}
		printf("#%d %d\n", tc, answer);
	}
}