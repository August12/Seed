// 백준 2667번

#include <stdio.h>

int MAP[25][25] = {0};
int VISIT[25][25] = {0};
int HOUSE[320] = {0};
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, 1, -1};
int count = 0;

void dfs(int y, int x) {
	int i = 0;
	int ny = 0;
	int nx = 0;

	count++;
	VISIT[y][x] = 1;
	for(i = 0;i < 4;i++) {
		ny = y + dy[i];
		nx = x + dx[i];

		if(ny >= 0 && nx >= 0 && ny <= 24 && nx <= 24) {
			if (MAP[ny][nx] == 1 && VISIT[ny][nx] == 0)
				bfs(ny,nx);
		}
	}
}

// 정렬 함수를 사용 하지 않고 직접 구현하는 방법
// 집의 갯수를 센 뒤 바로 배열 안의 값과 비교하여 삽입한다.
void insertCount(int count) {
	int index = 0; // 새로 삽입되는 index
	int len = 0; // 배열에 들어있는 실제 값들의 갯수
	int tmp_len = 0; // len 값을 사용하려는 변수
	int tmp = 0;
	int diff_index_len = 0; // different -> diff
	
	while(1) {
		if(HOUSE[len] == 0)
			break;
		else
			len++;
	}

	tmp_len = len;
	while(tmp_len>0) {
		if(count > HOUSE[index]) {
			index++;
			tmp_len--;
		}
		else
			break;
	}

	if(len == 0) {
		HOUSE[0] = count;
	}
	else if(len > 0) {
		diff_index_len = len - index;
		if(diff_index_len == 0)
			HOUSE[index] = count;
		else {
			while(index<len) {
				if(len == index+1) {
					tmp = HOUSE[len-1];
					HOUSE[len] = tmp;
					HOUSE[index] = count;
					len--;
				}
				else {
					tmp = HOUSE[len-1];
					HOUSE[len] = tmp;
					HOUSE[len-1] = HOUSE[len-2];
					len--;
				}				
			}
		}

	}
}

int main(void) {
	int HOUSE_LEN = 0;
	int N;
	int i,j;

	scanf("%d",&N);

	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			scanf("%1d",&MAP[i][j]);
		}
	}

	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			if(MAP[i][j] == 1 && VISIT[i][j] == 0) {
				bfs(i,j);
				insertCount(count);
				count = 0;
			}
		}
	}

	while(1) {
		if(HOUSE[HOUSE_LEN] == 0)
			break;
		else
			HOUSE_LEN++;
	}

	printf("%d\n", HOUSE_LEN);
	for(i=0;i<HOUSE_LEN;i++) {
		printf("%d\n", HOUSE[i]);
	}

	return 0;
}