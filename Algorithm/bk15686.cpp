#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int N, M;
int CITY[51][51];
int VSIT[51][51];
int Answer = 1e9;

vector < pair<int,int> > v;
vector < pair<int,int> > h;

void solution() {
	int sum = 0;

	for(int i=0; i<h.size(); i++) {
		int distance = 1e9;
		for(int j=0; j<v.size(); j++) {

			int tmp = abs(h[i].first - v[j].first) + abs(h[i].second - v[j].second);
			distance = (distance < tmp) ? distance : tmp;
		}
		sum += distance;
	}

	Answer = (Answer < sum) ? Answer : sum;
}

void dfs(int y, int x, int depth) {
	if(depth == M) {
		for(int i=0; i<v.size(); i++) {
			printf("(%d %d) ", v[i].first, v[i].second);
		}
		printf("\n");
		solution();
	}
	else {
		for(int r=y; r<=N; r++) {
			for(int c=1; c<=N; c++) {

				if(VSIT[r][c] == 0 && CITY[r][c] == 2) {
					VSIT[r][c] = 1;
					v.push_back(make_pair(r,c));
					dfs(r, c, depth+1);
					v.pop_back();
					VSIT[r][c] = 0;
				}
			}
		}
	}
}


int main() {
	scanf("%d %d", &N, &M);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=N; c++) {
			scanf("%d", &CITY[r][c]);
			if(CITY[r][c] == 1) {
				h.push_back(make_pair(r,c));
			}
		}
	}

	dfs(0,0,0);

	printf("%d\n", Answer);
}
