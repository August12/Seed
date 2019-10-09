#include <stdio.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

int N, M;
int map[1001][1001];
int parent[1001];

typedef struct {
	int a;
	int b;
	int c;
}Node;
vector <Node> v;

bool comp(const Node a, const Node b) {
	return a.c < b.c;
}

int main(void) {
	scanf("%d %d",&N,&M);
	for(int i=0; i<M; i++) {
		int a, b, c;
		scanf("%d %d %d",&a,&b,&c);

		Node tmp;
		tmp.a = a;
		tmp.b = b;
		tmp.c = c;
		v.push_back(tmp);
	}
	sort(v.begin(),v.end(),comp);

	for(int i=0; i<v.size(); i++) {
		printf("%d ",v[i].c);
	}
}
