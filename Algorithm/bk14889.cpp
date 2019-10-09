#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int N;
int sel[21];
int score[21][21];
int Answer;

vector < int > v;
vector < int > v2;

void diff(void) {
	int sum1 = 0;
	int sum2 = 0;
	int diff = 0;

	for(int i=0; i<v.size(); i++) {
		for(int j=0; j<v.size(); j++) {
			sum1 += score[v[i]][v[j]];
		}
	}

	for(int i=0; i<v2.size(); i++) {
		for(int j=0; j<v2.size(); j++) {
			sum2 += score[v2[i]][v2[j]];
		}
	}
	diff = abs(sum1 - sum2);

	Answer = Answer > diff ? diff : Answer;
}

void team(int man, int num) {
	if(num == N/2) {
		for(int pos=1; pos<=N; pos++) {
			if(sel[pos] == 0) {
				v2.push_back(pos);
			}
		}

		// for(int i=0; i<v.size(); i++) {
		// 	printf("%d ", v[i]);
		// }
		// printf("\n");

		diff();
		v2.clear();
	}
	else {
		for(int j=man+1; j<=N; j++) {
			if(sel[j] == 0) {
				sel[j] = 1;
				v.push_back(j);

				team(j,num+1);
				
				sel[j] = 0;
				v.pop_back();
			}
		}
	}
}

int main() {
	scanf("%d", &N);
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			scanf("%d", &score[i][j]);
		}
	}
	Answer = 1e9;

	v.push_back(1);
	sel[1] = 1;
	team(1, 1);
	printf("%d\n", Answer);
}