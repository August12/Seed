#include <stdio.h>
#include <vector>

using namespace std;

int T;
int A, B;
int is_Answer;

vector < long long > v;

void dfs(long long num) {
	if(num == B) {
		is_Answer = 1;
	}
	else {
		if(num*2 <= B) {
			v.push_back((int)num*2);
			dfs((int)num*2);
			if(is_Answer == 1) {
				return;
			}
			else {
				v.pop_back();
			}
		}

		if(num*10+1 <= B) {
			v.push_back((int)num*10+1);
			dfs((int)num*10+1);
			if(is_Answer == 1) {
				return;
			}
			else {
				v.pop_back();
			}
		}
	}
}

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d", &A, &B);
		is_Answer = 0;
		v.clear();
		v.push_back(A);
		dfs(A);
		if(is_Answer == 1) {
			printf("#%d %lu\n", test_case, v.size());
			for(int i=0; i<v.size(); i++) {
				printf("%lld ", v[i]);
			}
			printf("\n");
		}
		else {
			printf("#%d -1\n",test_case);
		}
	}
	return 0;
}