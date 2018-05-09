#include <stdio.h>
#include <time.h>

int Test_case;
int t,i,j;
int N,M;
char Map[51][51];
char priority[4];
int maxtime = 0;
int start_y, start_x;
int ny, nx;
int dx[4];
int dy[4];
clock_t start,end;

void prior(void) {

	for(i=0;i<4;i++) {
		if(priority[i] == 'W') {
			dx[i] = 0;
			dy[i] = -1;
		}
		else if(priority[i] == 'A') {
			dx[i] = -1;
			dy[i] = 0;
		}
		else if(priority[i] == 'S') {
			dx[i] = 0;
			dy[j] = 1;
		}
		else if(priority[i] == 'D') {
			dx[i] = 1;
			dy[i] = 0;
		}
	}

}

void dfs(int y,int x) {

	
	maxtime++;
	Map[y][x] = '.';

	for(i=0;i<4;i++) {
		ny = y + dy[i];
		nx = x + dx[i];

		if(ny >= 0 && ny < N && ny >= 0 && ny < M) {
			if(Map[ny][nx] == '#') {
				dfs(ny,nx);
			}
		}
	}
	
	
}

int main(void) {

	scanf("%d",&Test_case);

	for(t=1;t<=Test_case;t++) {

		maxtime = 0;

		scanf("%d",&N);
		scanf("%d",&M);

		for(i=0;i<N;i++) {
			for(j=0;j<M;j++) {
				scanf("%1s",&Map[i][j]);
				fflush(stdin);	
			}
		}

		for(i=0;i<4;i++) {
			scanf("%1s",&priority[i]);
		}

		start = clock();

		for(i=0;i<N;i++) {
			for(j=0;j<M;j++) {
				if(Map[i][j] == 'S') {
					start_y = i;
					start_x = j;
					i = N;
					break;
				}
			}
		}

		prior();
		dfs(start_y,start_x);

		printf("%d\n",maxtime);

		end = clock();
		printf("%.3lf\n",(end-start)/(double)1000);


	}

	return 0;
}