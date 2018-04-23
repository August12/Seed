#include <stdio.h>

int N,M;
int Map[100][100];
int visit[100][100];
int dx[] = {-1,1,0,0};
int dy[] = {0,0,1,-1};

typedef struct {
	int x;
	int y;
}Node;
Node queue[10000];

void bfs(Node n) {
	int front=0,rear=0;
	int nx,ny;
	int i;
	Node pop;

	visit[n.x][n.y] = 1;
	queue[rear] = n;
	rear++;

	while(front<rear) {
		pop = queue[front];
		front++;

		for(i=0;i<4;i++) {
			nx = pop.x + dx[i];
			ny = pop.y + dy[i];

			if((nx >= 0 && nx <= N-1) && (ny >= 0 && ny <= M-1)) {
				if((Map[nx][ny] == 1) && (visit[nx][ny] == 0)) {
					visit[nx][ny] = visit[pop.x][pop.y] + 1;
					queue[rear].x = nx;
					queue[rear].y = ny;
					rear++;
				}
			}
		}

	}

	printf("%d", visit[N-1][M-1]);
}

int main() {
	int i,j;
	Node n = {0};


	scanf("%d %d",&N,&M);
	for(i=0;i<N;i++) {
		for(j=0;j<M;j++) {
			scanf("%1d",&Map[i][j]);
		}
	}

	bfs(n);

	return 0;
}