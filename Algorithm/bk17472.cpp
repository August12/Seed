#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <queue>
#define INF 987654321

using namespace std;

int N, M;
int map[11][11];
int vit[11][11];
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1};
int num = 1;
int island[7][7];
int vited[7];
vector <pair<pair<int,int>,int> > beach;
int answer;

typedef struct {
	int cost;
	int s;
}Node;
priority_queue <Node> pq;

bool operator<(Node t, Node u){
    return t.cost > u.cost;
}

void naming(int y, int x) {
	// printf("%d %d\n",y,x);
	for(int i=0; i<4; i++) {
		int ny = y + dy[i];
		int nx = x + dx[i];

		if(ny>=1 && ny<=N && nx>=1 && nx<=M) {
			if(vit[ny][nx] == 0 && map[ny][nx] == 1) {
				map[ny][nx] = num;
				vit[ny][nx] = 1;
				naming(ny,nx);
			}
		}
	}
}

void find_beach(void) {
	for(int y=1; y<=N; y++) {
		for(int x=1; x<=M; x++) {
			if(map[y][x] != 0) {
				for(int i=0; i<4; i++) {
					int ny = y + dy[i];
					int nx = x + dx[i];

					if(ny>=1 && ny<=N && nx>=1 && nx<=M) {
						if(map[ny][nx] == 0) {
							beach.push_back(make_pair(make_pair(y,x),map[y][x]));
							break;
						}
					}
				}
			}
		}
	}
}

void bridge(int y, int x, int dir, int id, int cnt) {
	if(map[y][x] != 0 && map[y][x] != id) {
		cnt -= 1;
		if(island[id][map[y][x]] > cnt && cnt != 1) {
			island[id][map[y][x]] = cnt;
			island[map[y][x]][id] = cnt;
		}
		return;
	}
	else {
		int ny = y + dy[dir];
		int nx = x + dx[dir];

		if(ny>=1 && ny<=N && nx>=1 && nx<=M) {
			if(map[ny][nx] != id) {
				bridge(ny, nx, dir, id, cnt+1);
			}
		}
	}
}

void minicost(void) {
	while(!pq.empty()) {

		int s  = pq.top().s;
		int ccost = pq.top().cost;
		// printf("s : %d, cost : %d\n",s, ccost);
		pq.pop();

		if(vited[s] == 1) continue;
		
		vited[s] = 1;
		answer += ccost;
		for(int i=1; i<num; i++) {
			if(island[s][i] != INF && vited[i] == 0) {
				Node tmp;
				tmp.s = i;
				tmp.cost = island[s][i];
				pq.push(tmp);
			}
		}
	}

}

int main(void) {
	scanf("%d %d", &N, &M);
	for(int y=1; y<=N; y++) {
		for(int x=1; x<=M; x++) {
			scanf("%d", &map[y][x]);
		}
	}

	for(int y=1; y<=N; y++) {
		for(int x=1; x<=M; x++) {
			if(map[y][x] == 1 && vit[y][x] == 0) {
				map[y][x] = num;
				vit[y][x] = 1;
				naming(y,x);
				num++;
			}
		}
	}

	for(int y=1; y<num; y++) {
		for(int x=1; x<num; x++) {
			island[y][x] = INF;
		}
	}

	find_beach();
	for(int i=0; i<beach.size(); i++) {
		for(int j=0; j<4; j++) {
			bridge(beach[i].first.first, beach[i].first.second, j, beach[i].second, 0);
		}
	}

	//printf("dasdfasd\n");
	// printf("\n");
	// for(int y=1; y<=N; y++) {
	// 	for(int x=1; x<=M; x++) {
	// 		printf("%d ", map[y][x]);
	// 	}
	// 	printf("\n");
	// }

	// for(int y=1; y<num; y++) {
	// 	for(int x=1; x<num; x++) {
	// 		printf("%d ", island[y][x]);
	// 	}
	// 	printf("\n");
	// }

	for(int i=1; i<num; i++) {
		if(island[1][i] != INF) {
			Node tmp;
			tmp.cost = island[1][i];
			tmp.s = i;

			vited[1] = 1;
			pq.push(tmp);
		}
	}
	minicost();

	for(int i=1; i<num; i++) {
		if(vited[i] == 0) {
			printf("-1\n");
			return 0;
		}
	}
	printf("%d\n", answer);

	

	

	// for(int i=0; i<beach.size(); i++) {
	// 	printf("%d %d %d\n", beach[i].first.first, beach[i].first.second, beach[i].second);
	// }
}