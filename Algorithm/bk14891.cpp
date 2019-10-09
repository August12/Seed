#include <stdio.h>
#include <vector>

using namespace std;

int K;
int machenic[5][9];
vector < pair<int,int> > v;
int Answer;

void right(int gear, int direct) {
	if(gear+1<=4) {
		if(machenic[gear][3] != machenic[gear+1][7]) {
			if(direct == 1) {
				right(gear+1, -1);
			}
			else {
				right(gear+1, 1);
			}
		}
	}

	if(direct == 1) {
		int tmp = machenic[gear][1];
		machenic[gear][1] = machenic[gear][8];
		machenic[gear][8] = machenic[gear][7];
		machenic[gear][7] = machenic[gear][6];
		machenic[gear][6] = machenic[gear][5];
		machenic[gear][5] = machenic[gear][4];
		machenic[gear][4] = machenic[gear][3];
		machenic[gear][3] = machenic[gear][2];
		machenic[gear][2] = tmp;
	}
	else {
		int tmp = machenic[gear][1];
		machenic[gear][1] = machenic[gear][2];
		machenic[gear][2] = machenic[gear][3];
		machenic[gear][3] = machenic[gear][4];
		machenic[gear][4] = machenic[gear][5];
		machenic[gear][5] = machenic[gear][6];
		machenic[gear][6] = machenic[gear][7];
		machenic[gear][7] = machenic[gear][8];
		machenic[gear][8] = tmp;
	}
}

void left(int gear, int direct) {
	if(gear-1>=1) {
		if(machenic[gear][7] != machenic[gear-1][3]) {
			if(direct == 1) {
				left(gear-1, -1);
			}
			else {
				left(gear-1, 1);
			}
		}
	}

	if(direct == 1) {
		int tmp = machenic[gear][1];
		machenic[gear][1] = machenic[gear][8];
		machenic[gear][8] = machenic[gear][7];
		machenic[gear][7] = machenic[gear][6];
		machenic[gear][6] = machenic[gear][5];
		machenic[gear][5] = machenic[gear][4];
		machenic[gear][4] = machenic[gear][3];
		machenic[gear][3] = machenic[gear][2];
		machenic[gear][2] = tmp;
	}
	else {
		int tmp = machenic[gear][1];
		machenic[gear][1] = machenic[gear][2];
		machenic[gear][2] = machenic[gear][3];
		machenic[gear][3] = machenic[gear][4];
		machenic[gear][4] = machenic[gear][5];
		machenic[gear][5] = machenic[gear][6];
		machenic[gear][6] = machenic[gear][7];
		machenic[gear][7] = machenic[gear][8];
		machenic[gear][8] = tmp;
	}
}

int main() {
	for(int i=1; i<=4; i++) {
		for(int j=1; j<=8; j++) {
			scanf("%1d", &machenic[i][j]);
		}
	}
	scanf("%d", &K);
	for(int k=1; k<=K; k++) {
		int number;
		int direct;
		scanf("%d %d", &number, &direct);
		v.push_back(make_pair(number,direct));
	}

	for(int i=0; i<v.size(); i++) {
		int gear = v[i].first;
		
		if(gear-1>=1) {
			if(machenic[gear][7] != machenic[gear-1][3]) {
				if(v[i].second == 1) {
					left(gear-1,-1);
				}
				else {
					left(gear-1,1);
				}
			}
		}
		if(gear+1<=4) {
			if(machenic[gear][3] != machenic[gear+1][7]) {
				if(v[i].second == 1) {
					right(gear+1, -1);
				}
				else {
					right(gear+1, 1);
				}
			}
		}

		if(v[i].second == 1) {
			int tmp = machenic[gear][1];
			machenic[gear][1] = machenic[gear][8];
			machenic[gear][8] = machenic[gear][7];
			machenic[gear][7] = machenic[gear][6];
			machenic[gear][6] = machenic[gear][5];
			machenic[gear][5] = machenic[gear][4];
			machenic[gear][4] = machenic[gear][3];
			machenic[gear][3] = machenic[gear][2];
			machenic[gear][2] = tmp;
		}
		else {
			int tmp = machenic[gear][1];
			machenic[gear][1] = machenic[gear][2];
			machenic[gear][2] = machenic[gear][3];
			machenic[gear][3] = machenic[gear][4];
			machenic[gear][4] = machenic[gear][5];
			machenic[gear][5] = machenic[gear][6];
			machenic[gear][6] = machenic[gear][7];
			machenic[gear][7] = machenic[gear][8];
			machenic[gear][8] = tmp;
		}
	}

	for(int i=1; i<=4; i++) {
		if(machenic[i][1] == 1) {
			if(i == 1) {
				Answer += 1;
			}
			else if(i == 2) {
				Answer += 2;
			}
			else if(i == 3) {
				Answer += 4;
			}
			else {
				Answer += 8;
			}
		}
	}

	printf("%d\n", Answer);
}