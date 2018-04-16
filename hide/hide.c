#include <stdio.h>

typedef struct {
	int cnt;
	int idx;
}Node;

int N,K;
Node line[100001];
int visit[100001];
int dx[] = {-1, 1, 0};

void bfs(int N) {

	int front = 0,rear = 0;
	Node pop;
	int nx,i;

	line[rear].cnt = 0;
	line[rear].idx = N;
	rear++;
	visit[N] = 1;

	while(front<rear) {

		pop = line[front];
		front++;

		if(pop.idx == K) {
			printf("%d", pop.cnt);
			return;
		}

		for(i=0;i<3;i++) {
			if(dx[i] != 0)
				nx = pop.idx + dx[i];
			else
				nx = 2 * pop.idx;

			if(0 <= nx && nx <= 100000 && visit[nx] == 0) {
				line[rear].idx = nx;
				line[rear].cnt = pop.cnt + 1;
				rear++;
				visit[nx] = 1;
			}
		}

	}
}



int main() {

	scanf("%d%d",&N,&K);

	if (N >= K) {
		printf("%d", N-K);
		return 0;
	}

	bfs(N);
	return 0;
}