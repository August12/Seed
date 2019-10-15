#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct {
	int q;
	vector<int> live;
	vector<int> die;
}Node;

int N, M, K;
Node map[11][11];
int add[11][11];
int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
long long answer;

int main(void) {
	scanf("%d %d %d",&N,&M,&K);
	for(int y=1; y<=N; y++) {
		for(int x=1; x<=N; x++) {
			scanf("%d",&add[y][x]);
			map[y][x].q = 5;
		}
	}
	for(int i=0; i<M; i++) {
		int x, y, tree;
		scanf("%d %d %d",&y,&x,&tree);
		map[y][x].live.push_back(tree);
	}

	
	while(K!=0) {

		//spring
		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				int size = map[y][x].live.size();
				int dsize = 0;

				if(size > 0) {

					sort(map[y][x].live.begin(),map[y][x].live.end());
					for(int s=0; s<size; s++) {
						if(map[y][x].q - map[y][x].live[s] < 0) {
							dsize = size - s;
							break;
						}
						else {
							map[y][x].q -= map[y][x].live[s];
							map[y][x].live[s]++;
						}
					}

					for(int s=0; s<dsize; s++) {
						int tree = map[y][x].live.back();

						map[y][x].live.pop_back();
						map[y][x].die.push_back(tree);
					}
				}
			}
		}

		// summer
		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				int dsize = map[y][x].die.size();

				if(dsize > 0) {
					for(int s=0; s<dsize; s++) {
						int add = (map[y][x].die[s]) / 2;

						map[y][x].q += add;
					}

					map[y][x].die.clear();
				}

			}
		}

		// autumn
		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				int size = map[y][x].live.size();

				if(size > 0) {
					for(int s=0; s<size; s++) {
						if((map[y][x].live[s])%5 == 0) {
							for(int i=0; i<8; i++) {
								int ny = y + dy[i];
								int nx = x + dx[i];

								if(ny>=1 && nx>=1 && ny<=N && nx<=N) {
									map[ny][nx].live.push_back(1);
								}
							}
						}
					}
				}
			}
		}

		// winter
		for(int y=1; y<=N; y++) {
			for(int x=1; x<=N; x++) {
				map[y][x].q += add[y][x];
			}
		}

		K--;
	}

	for(int y=1; y<=N; y++) {
		for(int x=1; x<=N; x++) {
			answer += map[y][x].live.size();
		}
	}
	printf("%lld\n", answer);
}
