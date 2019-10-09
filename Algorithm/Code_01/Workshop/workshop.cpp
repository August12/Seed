/*
	1. dfs로 숙소로 k개수만큼 선택해서 vector에 넣는다.
	2. 낚시터의 좌표랑 vector에 있는 숙소 좌표랑 계산한다.
	3. 더 작은 거리를 더한다.
*/

#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

int T;
int N, M, K;
int Answer;

vector < pair<int,int> > F;
vector < pair<int,int> > H;
int D[101][14];
vector < int > selected;

void dfs(int idx, int cnt) {
	if(cnt == K) {
		int tot_dist = 0;

		for(int i=0; i<F.size(); i++) {
			int sub_dist = 1e9;
			for(int j=0; j<selected.size(); j++) {
				int house = selected[j];
				if(sub_dist > D[i][house]) {
					sub_dist = D[i][house];
				}
			}
			tot_dist += sub_dist;
		}
		if(Answer > tot_dist) {
			Answer = tot_dist;
		}
	}
	else {
		for(int i=0; i<H.size(); i++) {
			selected.push_back(i);
			dfs(i+1, cnt+1);
			selected.pop_back();
		}
	}
}

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d %d", &N, &M, &K);
		F.clear();
		H.clear();
		selected.clear();
		Answer = 1e9;
		for(int i=1; i<=N; i++) {
			for(int j=1; j<=N; j++) {
				int Map;
				scanf("%d",&Map);
				if(Map == 1) {
					F.push_back(make_pair(i,j));
				}
				else if(Map == 2) {
					H.push_back(make_pair(i,j));
				}
			}
		}
		for(int i=0; i<F.size(); i++) {
			for(int j=0; j<H.size(); j++) {
				D[i][j] = abs(F[i].first - H[j].first)+ abs(F[i].second - H[j].second);
			}
		}
		dfs(0,0);
		printf("#%d %d\n", test_case, Answer);
	}
}