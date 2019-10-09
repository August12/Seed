#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int N, L, R;
int Map[51][51];
int Vit[51][51];
int dr[4] = {1, 0, -1, 0};
int dc[4] = {0, 1, 0, -1};
int Cnt;
int Answer;

vector < pair<int,int> > v;

void dfs(int r, int c) {
	Vit[r][c] = 1;
	v.push_back(make_pair(r,c));

	for(int i=0; i<4; i++) {
		int nr = r + dr[i];
		int nc = c + dc[i];

		if(nr>=1 && nr<=N && nc>=1 && nc<=N) {
			if(Vit[nr][nc] == 0) {
				int diff = abs(Map[r][c] - Map[nr][nc]);

				if(diff>=L && diff<=R) {
					dfs(nr,nc);
				}
			}
		}
	}
}

int main() {
	scanf("%d %d %d", &N, &L, &R);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=N; c++) {
			scanf("%d", &Map[r][c]);
		}
	}

	while(1) {

		Cnt = 0;
		for(int r=1; r<=N; r++) {
			for(int c=1; c<=N; c++) {
				Vit[r][c] = 0;
			}
		}

		for(int r=1; r<=N; r++) {
			for(int c=1; c<=N; c++) {
				v.clear();
				
				if(Vit[r][c] == 0) {
					dfs(r,c);
				}

				if(v.size() > 1) {
					int sum = 0;
					for(int i=0; i<v.size(); i++) {
						int y = v[i].first;
						int x = v[i].second;

						sum += Map[y][x];
					}

					sum = sum/v.size();

					for(int i=0; i<v.size(); i++) {
						int y = v[i].first;
						int x = v[i].second;
						Map[y][x] = sum;
					}
					Cnt++;
				}

				if(Cnt > 0) {
					Cnt = 1;
				}
			}
		}

		if(Cnt == 0) break;
		else Answer += Cnt;
	}

	printf("%d\n", Answer);
}