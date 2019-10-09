#include <stdio.h>
#include <queue>

using namespace std;

int T, N, M;
int SY, SX, EY, EX;
int MAP[1001][1001];
int VIT[1001][1001];
int dy[4] = {0, 1, 0, -1};
int dx[4] = {1, 0, -1, 0};
int ANS;

queue < pair< pair<int,int>, int> > q;

int main(void) {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {

		for(int i=1; i<=N; i++) {
			for(int j=1; j<=M; j++) {
				MAP[i][j] = 0;
				VIT[i][j] = 0;
			}
		}

		while(!q.empty())
			q.pop();

		scanf("%d %d", &N, &M);
		scanf("%d %d %d %d", &SY, &SX, &EY, &EX);
		for(int i=1; i<=N; i++) {
			for(int j=1; j<=M; j++) {
				scanf("%d", &MAP[i][j]);
			}
		}

		q.push(make_pair(make_pair(SY,SX),0));
		VIT[SY][SX] = 1;
		ANS = -1;

		while(!q.empty()) {
			int y = q.front().first.first;
			int x = q.front().first.second;
			int cnt = q.front().second;
			q.pop();

			if(y == EY && x == EX) {
				ANS = cnt;
				break;
			}
			else {
				for(int i=0; i<4; i++) {
					int ny = y + dy[i];
					int nx = x + dx[i];
					int n_cnt = cnt + 1;

					if(ny>=1 && ny<=N && nx>=1 && nx<=M) {
						if(MAP[ny][nx] == 0 && VIT[ny][nx] == 0) {
							q.push(make_pair(make_pair(ny,nx),n_cnt));
							VIT[ny][nx] = 1;
						}
					}
				}
			}
		}
		printf("#%d %d\n", test_case, ANS);
	}
}

