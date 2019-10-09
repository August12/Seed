#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
// 문자열 입력 값 체크해서 공부하기.

using namespace std;

int T, N;
string card[100001];
int visit[100001];
string str;
int Answer;

void dfs(int depth) {
	if(depth == N+1) {
		// cout << str << endl;
		int w = 0;
		int t = 0;
		for(int i=str.size()-1; i>=1; i--) {
			if(str[i] == 'W') {
				w++;
			}
			else {
				t = t+w;
			}
		}
		Answer = (Answer < t) ? t : Answer;
	}
	else {
		for(int i=1; i<=N; i++) {
			if(visit[i] == 0) {
				printf("%d\n", i);
				visit[i] = 1;
				str = str+card[i];
				dfs(depth+1);
				visit[i] = 0;
				str.erase(str.end()-card[i].size(),str.end());
			}
		}
	}
}

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		string tmp;
		swap(str, tmp);
		Answer = 0;

		scanf("%d", &N);
		for(int i=1; i<=N; i++) {
			cin >> card[i];
		}
		dfs(1);
		printf("#%d %d\n", test_case, Answer);
	}
}