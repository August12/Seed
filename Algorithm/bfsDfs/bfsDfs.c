#include <stdio.h>

int Graph[1001][1001];
int bfsVisit[1001];
int dfsVisit[1001];

int queue[1001];

void DFS(int v,int N) {
	int i;

	dfsVisit[v]=1;
	printf("%d ", v); // 방문 노드 표시.
	for(i=0;i<=N;i++) {
		if(Graph[v][i] == 1 && dfsVisit[i] == 0)
			DFS(i,N);
	}
	return;
}

void BFS(int v,int N) {
	int front=0,rear=0,Pop,i;

	printf("%d ", v);
	queue[0]=v;
	rear++;
	bfsVisit[v]=1;

	while(front<rear) {
		Pop=queue[front];
		front++;

		for(i=1;i<=N;i++) {
			if(Graph[Pop][i] == 1 && bfsVisit[i] == 0) {
				printf("%d ", i);
				queue[rear] = i;
				rear++;
				bfsVisit[i]=1;
			}
		}
	}
	return;
}

int main(void) {
	int N,M,Start;
	int i,x,y;

	scanf("%d%d%d",&N,&M,&Start);

	for(i=0;i<M;i++) {
		scanf("%d%d",&x,&y);
		Graph[x][y] = 1;
		Graph[y][x] = 1;
	}

	DFS(Start,N);
	printf("\n");
	BFS(Start,N);
}