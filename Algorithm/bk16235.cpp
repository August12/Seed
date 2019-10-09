#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct land {
	int food;
	vector < pair<int,int> > tree;
} Land;

int N, M, K;
Land Map[11][11];
int Food[11][11];
int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int Answer;

bool comp(const pair<int,int> &a, const pair<int,int> &b) {
	if(a.first > b.first) {
		return a.first < b.first;
	}
	else {
		return a.first < b.first;
	}
}

int main() {
	scanf("%d %d %d", &N, &M, &K);

	for(int r=1; r<=N; r++) {
		for(int c=1; c<=N; c++) {
			scanf("%d", &Food[r][c]);
			Map[r][c].food = 5;
		}
	}
	for(int m=1; m<=M; m++) {
		int y, x, age;

		scanf("%d %d %d", &y, &x, &age);
		Map[y][x].tree.push_back(make_pair(age,1));
	}

	for(int k=1; k<=K; k++) {

		// spring
		for(int r=1; r<=N; r++) {
			for(int c=1; c<=N; c++) {

				if(Map[r][c].tree.size() > 0) {
					sort(Map[r][c].tree.begin(), Map[r][c].tree.end(), comp);

					for(int i=0; i<Map[r][c].tree.size(); i++) {
						
						if(Map[r][c].food - Map[r][c].tree[i].first >= 0) {
							Map[r][c].food -= Map[r][c].tree[i].first;
							Map[r][c].tree[i].first++;
						}
						else {
							Map[r][c].tree[i].second = 0;
						}
					}
				}
			}
		}

		// summer
		for(int r=1; r<=N; r++) {
			for(int c=1; c<=N; c++) {

				if(Map[r][c].tree.size() > 0) {

					for(int i=Map[r][c].tree.size()-1; i>=0; i--) {

						if(Map[r][c].tree[i].second == 0) {
							Map[r][c].food += Map[r][c].tree[i].first/2;
							Map[r][c].tree.pop_back();
						}
						else {
							break;
						}
					}
				}
			}
		}

		// autumn
		vector < pair<int,int> > _tree;
		for(int r=1; r<=N; r++) {
			for(int c=1; c<=N; c++) {

				if(Map[r][c].tree.size() > 0) {

					for(int i=0; i<Map[r][c].tree.size(); i++) {

						if(Map[r][c].tree[i].first % 5 == 0) {
							_tree.push_back(make_pair(r,c));
						}
					}
				}
			}
		}

		for(int i=0; i<_tree.size(); i++) {
			for(int j=0; j<8; j++) {
				int nr = _tree[i].first + dr[j];
				int nc = _tree[i].second + dc[j];

				if(nr>=1 && nr<=N && nc>=1 && nc<=N) {
					Map[nr][nc].tree.push_back(make_pair(1,1));
				}
			}
		}
		_tree.clear();

		// winter
		for(int r=1; r<=N; r++) {
			for(int c=1; c<=N; c++) {
				Map[r][c].food += Food[r][c];
			}
		}
	}

	for(int r=1; r<=N; r++) {
		for(int c=1; c<=N; c++) {
			Answer += Map[r][c].tree.size();
		}
	}
	printf("%d\n", Answer);
}

