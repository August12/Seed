#include <stdio.h>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

int N;
int parent[100001];
vector<int> v[100001];

queue <pair<int,int> > q;

void bfs(void) {
	q.push(make_pair(1,1));
	while(!q.empty()) {
		int node = q.front().first;
		int pat = q.front().second;
		q.pop();

		for(int i=0; i<v[node].size(); i++) {
			if(v[node][i] != pat) {
				int target = v[node][i];
				parent[target] = node;
				q.push(make_pair(target,node));
			}
		}
	}
}



int main(void) {
	scanf("%d",&N);
	for(int i=0; i<N-1; i++) {
		int a, b;
		scanf("%d %d",&a,&b);
		v[a].push_back(b);
		v[b].push_back(a);
	}
	bfs();
	for(int i=2; i<=N; i++) {
		printf("%d\n", parent[i]);
	}
}