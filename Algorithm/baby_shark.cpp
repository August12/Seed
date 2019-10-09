#include <stdio.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

int N;
int MAP[21][21];
int VIT[21][21];
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, -1, 0, 1};
int BABY = 2;
int _size;
int Answer;

queue < pair< pair<int,int>, int> > q;
vector < pair< pair<int,int>, int> > v;

bool comp(const pair< pair<int,int>, int> &a, const pair< pair<int,int>, int> &b) {
	if(a.second == b.second) {
		if(a.first.first == b.first.first) {
			return a.first.second < b.first.second;
		}
		else {
			return a.first.first < b.first.first;
		}
	}
	else {
		return a.second < b.second;
	}
}

int main() {
	scanf("%d", &N);
	for(int row=1; row<=N; row++) {
		for(int col=1; col<=N; col++) {
			scanf("%d", &MAP[row][col]);
			if(MAP[row][col] == 9) {
				q.push(make_pair(make_pair(row,col),0));
				VIT[row][col] = 1;
				MAP[row][col] = 0;
			}
		}
	}

	while(true) {
		while(!q.empty()) {
			int y = q.front().first.first;
			int x = q.front().first.second;
			int time = q.front().second;
			q.pop();

			for(int i=0; i<4; i++) {
				int ny = y + dy[i];
				int nx = x + dx[i];

				if(nx>=1 && nx<=N && ny>=1 && ny<=N) {
					if(MAP[ny][nx]>=0 && MAP[ny][nx]<=BABY && VIT[ny][nx]==0) {
						q.push(make_pair(make_pair(ny,nx),time+1));
						VIT[ny][nx] = 1;
						if(MAP[ny][nx]>0 && MAP[ny][nx]<BABY) {
							v.push_back(make_pair(make_pair(ny,nx),time+1));
						}
					}
				}

			}
		}

		if(!v.empty()) {
			sort(v.begin(),v.end(),comp);
			while(!q.empty()) {
				q.pop();
			}
			for(int row=1; row<=N; row++) {
				for(int col=1; col<=N; col++) {
					VIT[row][col] = 0;
				}
			}
			int fy = v[0].first.first;
			int fx = v[0].first.second;
			int ft = v[0].second;
			v.clear();

			q.push(make_pair(make_pair(fy,fx),ft));
			MAP[fy][fx] = 0;
			VIT[fy][fx] = 1;
			_size++;
			if(_size == BABY) {
				BABY++;
				_size = 0;
			}
			Answer = ft;
		}
		else {
			break;
		}
	}
	printf("%d\n", Answer);
}