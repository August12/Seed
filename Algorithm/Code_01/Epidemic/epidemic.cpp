#include <stdio.h>
#include <queue>

using namespace std;

queue < pair<pair<int,int>,int> > q;

int T, N, M;
int Map[1001][1001];
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, -1, 0, 1};
int Answer;

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d", &N, &M);
		while(!q.empty()) {
			q.pop();
		}
		Answer = 0;

		for(int row=1; row<=N; row++) {
			for(int col=1; col<=N; col++) {
				scanf("%d", &Map[row][col]);

				if(Map[row][col] == 1) {
					q.push(make_pair(make_pair(row,col),0));
				}
			}
		}

		while(!q.empty()) {
			int y = q.front().first.first;
			int x = q.front().first.second;
			int day = q.front().second;
			q.pop();

			if(day >= M) {
				break;
			}

			for(int i=0; i<4; i++) {
				int ny = y + dy[i];
				int nx = x + dx[i];

				if(ny>=1 && nx>=1 && ny<=N && nx<=N) {
					if(Map[ny][nx] == 0) {
						q.push(make_pair(make_pair(ny,nx),day+1));
						Map[ny][nx] = 1;
					}
				}
			}
		}

		for(int row=1; row<=N; row++) {
			for(int col=1; col<=N; col++) {
				if(Map[row][col] == 1) {
					Answer++;
				}
			}
		}
		printf("#%d %d\n", test_case, Answer);
	}
}