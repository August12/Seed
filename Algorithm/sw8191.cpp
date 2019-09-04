#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

int T, N;
vector < pair<int,int> > v;

int main(void) {

	scanf("%d", &T);
	for(int tc=1; tc<=T; tc++) {

		scanf("%d", &N);
		for(int i=0; i<N; i++) {

			int tmp;
			scanf("%d", &tmp);

			v.push_back(make_pair(tmp,i));
		}

		sort(v.begin(),v.end());

		int count = 1;
		for(int i=0; i<N-1; i++) {

			if(v[i].second > v[i+1].second)
				count++;
			else
				continue;
		}

		printf("#%d %d\n", tc, count);
	}
}