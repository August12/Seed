#include <stdio.h>
#include <vector>

using namespace std;

int N;
int Num[11];
int Sel[10];
int MIN, MAX;

vector < char > v;
vector < char > v2;

void cal() {
	int sum;

	for(int i=0; i<N-1; i++) {
		if(i == 0) {
			if(v2[i] == '+') sum = Num[i] + Num[i+1];
			else if(v2[i] == '-') sum = Num[i] - Num[i+1];
			else if(v2[i] == '*') sum = Num[i] * Num[i+1];
			else sum = Num[i] / Num[i+1];
		}
		else {
			if(v2[i] == '+') sum += Num[i+1];
			else if(v2[i] == '-') sum -= Num[i+1];
			else if(v2[i] == '*') sum *= Num[i+1];
			else sum /= Num[i+1];
		}
	}

	MAX = MAX < sum ? sum : MAX;
	MIN = MIN > sum ? sum : MIN;
}

void dfs(int cnt) {
	if(cnt == N-1) {
		cal();
	}
	else {
		for(int i=0; i<N-1; i++) {
			if(Sel[i] == 0) {
				Sel[i] = 1;
				v2.push_back(v[i]);
				dfs(cnt+1);
				Sel[i] = 0;
				v2.pop_back();
			}
		}
	}
}

int main() {
	scanf("%d", &N);
	for(int i=0; i<N; i++) {
		scanf("%d", &Num[i]);
	}
	for(int i=0; i<4; i++) {
		int cnt;
		scanf("%d", &cnt);

		for(int c=1; c<=cnt; c++) {
			if(i == 0) v.push_back('+');
			else if(i == 1) v.push_back('-');
			else if(i == 2) v.push_back('*');
			else v.push_back('/');
		}
	}

	MIN = 1e9;
	MAX = -1e9;

	dfs(0);

	printf("%d\n", MAX);
	printf("%d\n", MIN);
} 