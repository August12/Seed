#include <stdio.h>

int MAP[100][100] = {0};
int COUNT = 0;
typedef struct {
	int y;
	int x;
} NODE;
int dy[4] = {1, 0, -1, 0};
int dx[4] = {0, -1, 0, 1};

void draw_curve(NODE start_node,int direction,int max_generation) {
	int QUEUE[512] = {0};
	int i,j;
	int front = 0,rear = 0;
	int generation = 0;
	int target = 0;
	NODE node = start_node;

	for(i = max_generation;i >= 0;i--) {
		// rear를 사용하는 이유 : 큐의 현재 길이이기 때문에
		for(j = rear;j > 0;j--) {

			QUEUE[rear] = direction;
			printf("------\n");
			printf("%d\n", QUEUE[rear]);
			rear++;
			direction++;
			printf("------\n");

			if(direction == 4)
				direction = 0;
		}
		//처음 0세대인 경우
		if(rear == 0) {
			QUEUE[rear] = direction;
			printf("------\n");
			printf("%d\n", QUEUE[rear]);
			rear++;
			direction++;
			printf("------\n");

			if(direction == 4)
				direction = 0;
		}
	}

	while(front<=rear) {
		if(node.y >= 0 && node.x >= 0 && node.y < 101 && node.x < 101) {
			printf("------\n");
			printf("%d,%d\n", node.x,node.y);
			MAP[node.y][node.x] = 1;
			printf("%d\n", front);
			target = QUEUE[front];
			printf("%d\n", target);
			printf("------\n");
			front++;
			node.y += dy[target];
			node.x += dx[target];
		}
		else
			break;
	}
}

int main(void) {
	int N;
	int i,j;
	int x,y,d,g;

	scanf("%d",&N);
	for(i = 0; i < N; i++) {
		scanf("%d",&y);
		scanf("%d",&x);
		scanf("%d",&d);
		scanf("%d",&g);

		NODE start_node;
		start_node.y = y;
		start_node.x = x;

		draw_curve(start_node,d,g);
	}

	for(i = 0;i < 10;i++) {
		for(j = 0;j < 10;j++) {
			printf("%d", MAP[j][i]);
		}
		printf("\n");
	}
	
}