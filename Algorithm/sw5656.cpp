#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <string.h>

using namespace std;

int T, N, W, H;
int map[16][13];
int cpy[16][13];
int answer;
vector <int> v;

void start(int w) {
	queue <pair<pair<int,int>,int> > q;
	
	for(int h=1; h<=H; h++) {
		if(cpy[h][w] != 0) {
			q.push(make_pair(make_pair(h,w),cpy[h][w]));
			cpy[h][w] = 0;
			break;
		}
	}

	while(!q.empty()) {
		int ch = q.front().first.first;
		int cw = q.front().first.second;
		int cover = q.front().second;
		q.pop();

		for(int j=1; j<=cover-1; j++) {

			if(cw+j<=W) {
				if(cpy[ch][cw+j] > 1) {
					q.push(make_pair(make_pair(ch,cw+j),cpy[ch][cw+j]));
				}
				cpy[ch][cw+j] = 0;
			}

			if(cw-j>=1) {
				if(cpy[ch][cw-j] > 1) {
					q.push(make_pair(make_pair(ch,cw-j),cpy[ch][cw-j]));
				}
				cpy[ch][cw-j] = 0;
			}

			if(ch+j<=H) {
				if(cpy[ch+j][cw] > 1) {
					q.push(make_pair(make_pair(ch+j,cw),cpy[ch+j][cw]));
				}
				cpy[ch+j][cw] = 0;
			}

			if(ch-j>=1) {
				if(cpy[ch-j][cw] > 1) {
					q.push(make_pair(make_pair(ch-j,cw),cpy[ch-j][cw]));
				}
				cpy[ch-j][cw] = 0;
			}
		}
	}

	return;
}

void update(void) {
	int idx = 0;
	int flag = 0;

	for(int w=1; w<=W; w++) {
		flag = 0;
		for(int h=H; h>=1; h--) {
			if(flag == 0 && cpy[h][w] == 0) {
				idx = h;
				flag = 1;
			}
			else if(flag == 1 && cpy[h][w] != 0) {
				cpy[idx][w] = cpy[h][w];
				idx--;
				cpy[h][w] = 0;
			}
		}
	}

	return;
}

void find(void) {
	int ret = 0;
	for(int h=1; h<=H; h++) {
		for(int w=1; w<=W; w++) {
			if(cpy[h][w] != 0) ret++;
		}
	}

	if(answer > ret) {
		answer = ret;
	}
	return;
}


void dfs(int depth) {
	if(depth > N) {
		for(int h=1; h<=H; h++) {
			for(int w=1; w<=W; w++) {
				cpy[h][w] = map[h][w];
			}
		}

		for(int k=0; k<N; k++) {
			start(v[k]);
			update();
		}
		find();

		return;
	}
	else {
		for(int i=1; i<=W; i++) {
			v.push_back(i);
			dfs(depth+1);
			v.pop_back();
		}
	}
}

int main(void) {
	//freopen("sample_input.txt","r",stdin);

	scanf("%d",&T);
	for(int tc=1; tc<=T; tc++) {
		scanf("%d %d %d", &N, &W, &H);
		for(int y=1; y<=H; y++) {
			for(int x=1; x<=W; x++) {
				scanf("%d",&map[y][x]);
			}
		}
		v.clear();
		answer = 1e9;

		dfs(1);

		printf("#%d %d\n", tc, answer);
	}
}