#include <stdio.h>

int Map[26][26];
int Visit[26][26];
int N;
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, 1, -1};
int idx = 1;
int ans[20];
int cnt;

void dfs(int y,int x) {
	int i;
	int ny,nx;

	if(Visit[ny][nx] == 1) return;

	Map[y][x] = idx;
	Visit[y][x] = 1;
	cnt++;

	for(i=0;i<4;i++) {
		ny = y + dy[i];
		nx = x + dx[i];

		if(ny>=0 && ny<N && nx>=0 && nx<N) {
			if(Map[ny][nx]==1 && Visit[ny][nx]==0) {
				dfs(ny,nx);
			}
		}
	}
}

void quicksort(int ans[],int low,int high) {
	int pivot = ans[(high-low)/2];
	int left = low;
	int right = high;
	int tmp;

	while(left<right) {
		while(pivot>ans[left]) left++;
		while(pivot<ans[right]) right--;

		if(left<right) {
			tmp = ans[left];
			ans[left] = ans[right];
			ans[right] = tmp;
		}

		if(left>right) {
			tmp = ans[right];
			ans[right] = pivot;
			pivot = tmp;

			quicksort(ans,low,right-1);
			quicksort(ans,right+1,high);
		}
	}

}


int main(void) {
	int i,j;

	scanf("%d",&N);
	for(i=0;i<N;i++) {
		for(j=0;j<N;j++) {
			scanf("%1d",&Map[i][j]);
		}
	}

	for(i=0;i<N;i++) {
		for(j=0;j<N;j++) {
			if(Map[i][j]==1 && Visit[i][j]==0) {
				dfs(i,j);
				ans[idx] = cnt;
				cnt = 0;
				idx++;
			}
		}
	}

	quicksort(ans,1,idx-1);

	printf("%d\n", idx-1); //단지 수 출력
	for(i=1;i<idx;i++) printf("%d\n", ans[i]);

	return 0;
}