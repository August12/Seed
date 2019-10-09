/*
	1. 드래곤을 파워로 오름차순 정렬 한다음에 번호를 따로 저장,
	2. 잡는 순서대로 큐에 넣어준다.
	3. 못 잡으면 -1을 출력하고 프로그램 종료.
	4. 다 잡으면 큐를 출력해준다.
*/

#include <stdio.h>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

typedef struct _Dragon {
	int num;
	int power;
	int bonus;
} Dragon;

vector <Dragon> vec;
queue <int> que;

bool comp(const Dragon &a, const Dragon &b) {
	if(a.power == b.power) return a.bonus < b.bonus;
	else return a.power < b.power;
}

int T;
int P, N;

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		while(!que.empty()) {
			que.pop();
		}
		vec.clear();

		scanf("%d %d", &P, &N);
		for(int i=1; i<=N; i++) {
			Dragon tmp;
			tmp.num = i;
			scanf("%d %d", &tmp.power, &tmp.bonus);
			vec.push_back(tmp);
		}

		sort(vec.begin(),vec.end(),comp);

		// for(int i=0; i<vec.size(); i++) {
		// 	printf("num   : %d\n", vec[i].num);
		// 	printf("power : %d\n", vec[i].power);
		// 	printf("bonus : %d\n", vec[i].bonus);
		// }

		while(!vec.empty()) {
			if(vec.front().power >= P) {
				break;
			}
			else {
				P += vec.front().bonus;
				que.push(vec.front().num);
				vec.erase(vec.begin());
			}
		}

		if(!vec.empty()) {
			printf("#%d %d\n", test_case, -1);
		}
		else {
			printf("#%d", test_case);
			while(!que.empty()) {
				printf(" %d", que.front());
				que.pop();
			}
			printf("\n");
		}
	}
	return 0;
}
/*
7
10 1
100 100
2 2
1 99
100 0
10 10
2 10
3 10
4 10
2 20
3 20
3 20
100 50
100 30
150 30
200 10
100 5
99 100
199 1
199 1
199 1
202 1
5 10
20 1
4 3
5 1
100 1
4 2
101 1
10 0
10 2
17 3
12 84
10 4
20 1
3 5
2 4
1 3
10 10
2 10
3 10
4 10
2 20
3 20
3 20
100 50
100 30
150 30
200 10
*/
