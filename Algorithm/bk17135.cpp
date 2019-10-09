#include <stdio.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

int N, M, D;
int MAP[17][16];
int CPY[17][16];
int dy[3] = {-1, 0, 0};
int dx[3] = {0, 1, -1};
int Ans;

vector < pair<int,int> > archer;
vector < pair<int,int> > die;

bool comp(const pair<int,pair<int,int> > &a, const pair<int,pair<int,int> > & b) {
	if(a.first == b.first) {
		return a.second.second < b.second.second;
	}
	else {
		return a.first < b.first;
	}
}

int check(void) {
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			if(CPY[r][c] == 1) return 1;
		}
	}
	return 0;
}

void bfs(int y, int x) {
	queue < pair<pair<int,int>,int> > q;
	vector < pair<int,pair<int,int> > > v;
	int VIT[17][16] = {0,};

	q.push(make_pair(make_pair(y,x),0));

	while(!q.empty()) {
		int _y = q.front().first.first;
		int _x = q.front().first.second;
		int d = q.front().second;
		q.pop();

		if(d == D) break;

		for(int i=0; i<3; i++) {
			int ny = _y + dy[i];
			int nx = _x + dx[i];

			if(ny>=1 && ny<=N && nx>=1 && nx<=M) {
				if(CPY[ny][nx] == 1 && VIT[ny][nx] == 0) {
					// 후보자 추가
					v.push_back(make_pair(d+1,make_pair(ny,nx)));
					VIT[ny][nx] = 1;
				}
				else if(CPY[ny][nx] == 0 && VIT[ny][nx] == 0) {
					q.push(make_pair(make_pair(ny,nx),d+1));
					VIT[ny][nx] = 1;
				}
			}
		}
	}
	if(!v.empty()) {
		sort(v.begin(),v.end(),comp);
		die.push_back(make_pair(v[0].second.first,v[0].second.second));
	}
}

void dfs(int idx, int cnt) {
	if(cnt == 4) {

		for(int r=1; r<=N; r++) {
			for(int c=1; c<=M; c++) {
				CPY[r][c] = MAP[r][c];
			}
		}

		int kill = 0;

		//궁수 배치 완료 후, 게임시작.
		while(true) {

			// 적이 없으면 종료.
			if(!check()) {
				Ans = Ans < kill ? kill : Ans;
				break;
			}

			// 가장 가까우면서 왼쪽에 있는 적군 추가
			for(int i=0; i<archer.size(); i++) {
				bfs(archer[i].first,archer[i].second);
			}


			// 적군 제거.
			for(int i=0; i<die.size(); i++) {
				int py = die[i].first;
				int px = die[i].second;

				// 이미 죽였는지 확인.
				if(CPY[py][px] == 1) {
					CPY[py][px] = 0;
					kill++;
				}
			}
			die.clear();

			// 맵에 남아있는 적 전진
			for(int r=N; r>=1; r--) {
				for(int c=1; c<=M; c++) {
					if(CPY[r][c] == 1) {

						CPY[r][c] = 0;
						if(r+1 <= N) CPY[r+1][c] = 1;
					}
				}
			}
		}
	}
	else {
		for(int i=idx; i<=M; i++) {
			archer.push_back(make_pair(N+1,i));
			dfs(i+1,cnt+1);
			archer.pop_back();
		}
	}
}

int main(int argc, char const *argv[])
{
	scanf("%d %d %d", &N, &M, &D);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			scanf("%d", &MAP[r][c]);
		}
	}

	dfs(1,1);
	printf("%d\n", Ans);

	return 0;
}