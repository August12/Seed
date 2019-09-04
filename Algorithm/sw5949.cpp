#include <iostream>
#include <string>

#define MAX 100000

using namespace std;

int T;
char S[MAX], A[MAX];

long long Answer;

int backSearch(int idx) {
	int cnt = 0;
	for(int i=idx+1; S[i]!='0'; i++) {
		if(A[idx] == S[i]) return ++cnt;
		else cnt++;
	}

	return cnt;
}

void backSwap(int idx, int cnt) {

	while(cnt > 0) {
		char tmp;

		tmp = S[idx];
		S[idx] = S[idx-1];
		S[idx-1] = tmp;
		cnt--;
		idx -= 1;
	}
}

int main(void) {
	scanf("%d", &T);

	for(int tc=1; tc<=T; tc++) {
		for(int i=0; i<MAX; i++) {
			S[i] = '0';
			A[i] = '0';
		}
		Answer = 0;

		scanf("%s",S);
		scanf("%s",A);

		//printf("S : %s A : %s\n", S, A);

		for(int i=0; S[i]!='0'; i++) {
			//printf("i : %d, S[i] : %c, A[i] : %c\n", i, S[i], A[i]);

			if(S[i] != A[i]) {
				int bcnt = backSearch(i);

				int idx = i + bcnt;
				backSwap(idx, bcnt);
				//printf("bcnt : %d, idx : %d, %s\n", bcnt, idx, S);
				Answer += bcnt;
			}
		}
		printf("#%d %lld\n", tc, Answer);
	}
}


abaa
baaa


aaaaabaaabaa
aaaaaaaaaabb
