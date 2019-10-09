#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

int main() {
	int T;
	int score[6];

	freopen("random_input.txt", "r", stdin);

	scanf("%d",&T);

	for(int test_case=0;test_case<T;test_case++) {
		int answer = 0;
		for(int i=0;i<6;i++) {
			scanf("%d",&score[i]);
		}

		sort(score,score+6);
		
		for(int i=1;i<5;i++) {
			answer += score[i];
		}
		answer /= 4;

		printf("#%d %d\n", test_case+1,answer);
	}
}