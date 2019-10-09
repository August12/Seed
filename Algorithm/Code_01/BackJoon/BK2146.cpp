/* 다리 만들기
	1. 모든 섬의 가장자리를 찾는다.
	2. 모든 섬의 가장자리에서 bfs를 돌린다.
	3. Answer를 출력한다.
*/

#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;

int N;
int Map[1001][1001];
int Visited[1001][1001];
int dy[4] = {1, 0, -1, 0};
int dx[4] = {0, 1, 0, -1};
int Island;
int Answer;

vector < pair<int,int> > edge;
queue < pair< pair< pair<int,int>,int>,int> > q;

void dfs(int y, int x) {
	if(Visited[y][x]==0) {
		Visited[y][x] = 1;
		Map[y][x] = Island;

		for(int i=0; i<4; i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];

			if(ny>=1 && ny<=N && nx>=1 && nx<=N) {
				if(Visited[ny][nx]==0 && Map[ny][nx]==1) {
					dfs(ny,nx);
				}
			}
		}
	}
}

void bfs() {
	while(!q.empty()) {
		int y = q.front().first.first.first;
		int x = q.front().first.first.second;
		int d = q.front().first.second;
		int cnt = q.front().second;
		q.pop();

		if(Map[y][x] != d && Map[y][x] != 0) {
			if(Answer > cnt) {
				Answer = cnt;
			}
			break;
		}

		for(int i=0; i<4; i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];

			if(ny>=1 && ny<=N && nx>=1 && nx<=N) {
				if(Visited[ny][nx]==0 && Map[ny][nx]!=d) {
					q.push(make_pair(make_pair(make_pair(ny,nx),d),cnt+1));
					Visited[ny][nx] = 1;
				}
			}
		}
	}

}

int main() {
	scanf("%d", &N);
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			scanf("%d", &Map[i][j]);
		}
	}

	// 가장자리 찾기.
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			if(Map[i][j]==1) {
				for(int k=0; k<4; k++) {
					int ni = i + dy[k];
					int nj = j + dx[k];

					if(ni>=1 && ni<=N && nj>=1 && nj<=N) {
						if(Map[ni][nj]==0) {
							edge.push_back(make_pair(i,j));
							break;
						}
					}
				}
			}
		}
	}

	// 섬 번호 붙이기.
	Island = 1;
	for(int i=0; i<edge.size(); i++) {
		if(Visited[edge[i].first][edge[i].second]==0) {
			dfs(edge[i].first, edge[i].second);
			Island++;
		}
	}

	// 모든 가장자리에서 bfs를 돌려 최소 다리 길이 찾기.
	Answer = 1000;
	for(int k=0; k<edge.size(); k++) {
		while(!q.empty()) {
			q.pop();
		}
		// Vistied 초기화.
		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				Visited[i][j] = 0;
			}
		}

		q.push(make_pair(make_pair(make_pair(edge[k].first,edge[k].second),Map[edge[k].first][edge[k].second]),-1));
		Visited[edge[k].first][edge[k].second] = 1;
		bfs();
	}

	printf("%d\n", Answer);
}