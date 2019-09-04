#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

int T;
int N, M;
vector< pair<int,pair<int,int> > > TABLE;

int main(void) {
	scanf("%d", &T);

	for(int tc=1; tc<=T; ++tc) {
		int sy, sx;
		scanf("%d %d", &N, &M);

		for(int y=1; y<=N; ++y) {
			getchar(); // 버퍼 초기화.
			for(int x=1; x<=M; ++x) {
				char tmp;
				scanf("%1c", &tmp);

				if(tmp == '*') {
					sy = y;
					sx = x;
				}
				else if(tmp == '#') {
					TABLE.push_back(make_pair(0,make_pair(y,x)));
				}
				else continue;
			}
		}

		for(int idx=0; idx<TABLE.size(); ++idx) {
			int ty, tx;
			int distance;

			ty = TABLE[idx].second.first;
			tx = TABLE[idx].second.second;
			distance = (sy-ty)*(sy-ty) + (sx-tx)*(sx-tx);
			TABLE[idx].first = distance;
		}

		sort(TABLE.begin(),TABLE.end());

		for(int i=0; i<TABLE.size(); ++i) {
			printf("%d, (%d, %d)\n", TABLE[i].first, TABLE[i].second.first, TABLE[i].second.second);
		}
	}
}