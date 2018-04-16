#include <stdio.h>

int Test_case;
int M,N,K;
int X,Y;
int Map[51][51];
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, -1, 1};
int visit[51][51];

void dfs(int y,int x) {
	int i,j;
	int ny,nx;

	for(i=0;i<4;i++) {
		ny = y + dy[i];
		nx = x + dx[i];

		if(ny>=0 && ny<N && nx>=0 && nx<M) {
			if(Map[ny][nx] == 1 && visit[ny][nx] == 0) {
				visit[ny][nx] = 1;
				dfs(ny,nx);
			}
		}
	}
}

int main() {

	int i,j,k;
	int ans=0;

	scanf("%d",&Test_case);

	for(i=0;i<Test_case;i++) {
		scanf("%d%d%d",&M,&N,&K);
		
		for(j=0;j<K;j++) {
			scanf("%d%d",&X,&Y);
			Map[Y][X] = 1;
		}

		for(j=0;j<N;j++) {
			for(k=0;k<N;k++) {
				if(Map[j][k] == 1 && visit[j][k] == 0) {
					ans++;
					visit[j][k] = 1;
					dfs(j,k);
				}
			}
		}

		printf("%d\n", ans);	
	}
}