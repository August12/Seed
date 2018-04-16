#include <stdio.h>

typedef struct
{
	int y;
	int x;
	int date;
	
}tomato;

int M,N;
int Box[1001][1001];
int visit[1001][1001];
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, 1, -1};

void bfs() {
	int i,j;
	tomato queue[1000001];
	int front=0,rear=0;
	tomato pop;
	int ny,nx;
	int max = 0;

	for(i=0;i<N;i++) {
		for(j=0;j<M;j++) {
			if(Box[i][j] == 1) {
				queue[rear].y = i;
				queue[rear].x = j;
				queue[rear].date = 0;
				rear++;
			}
		}
	}

	while(front<rear) {
		pop = queue[front];
		front++;
		visit[pop.y][pop.x] = 1;

		for(i=0;i<4;i++) {
			ny = pop.y + dy[i];
			nx = pop.x + dx[i];

			if(ny >= 0 && nx >= 0 && ny < N && nx < M) {
				if(Box[ny][nx] == 0 && visit[ny][nx] == 0) {
					queue[rear].y = ny;
					queue[rear].x = nx;
					queue[rear].date = pop.date + 1;
					Box[ny][nx] = 1;
					
					if(max < queue[rear].date) {
						max = queue[rear].date;
					}
					rear++;
				}
			}
		}
	}

	for(i=0;i<N;i++) {
		for(j=0;j<M;j++) {
			if(Box[i][j] == 0) {
				max = -1;
				break;
			}
		}
	}

	printf("%d\n", max);


}


int main() {

	int i,j;
	int max=0;

	scanf("%d%d",&M,&N);

	for(i=0;i<N;i++) {
		for(j=0;j<M;j++) {
			scanf("%d",&Box[i][j]);
		}
	}

	bfs();

	return 0;
}