#include <iostream>
#include <queue>

using namespace std;

int T;
int N;
long long M;
int Answer;

typedef struct {
	int num;
	int price;
}LAND;

int total_land[10001];
queue < LAND > select_land;

int main(void) {
	scanf("%d", &T);

	for(int tc=1; tc<=T; ++tc) {
		scanf("%d %lld", &N, &M);
		Answer = 0;

		for(int i=1; i<=N; ++i) scanf("%d", &total_land[i]);
		while(!select_land.empty()) select_land.pop();

		long long total_price = 0;
		int cnt = 1;

		while(cnt <= N+1) {
			if(total_price == M) {
				Answer++;
				cnt = select_land.back().num + 1;
				total_price -= select_land.front().price;
				select_land.pop();
			}
			else if(total_price > M && cnt <= N) {
				cnt = select_land.back().num + 1;
				total_price -= select_land.front().price;
				select_land.pop();
			}
			else if(total_price < M && cnt <= N) {
				LAND tmp;
				tmp.num = cnt;
				tmp.price = total_land[cnt];
				cnt++;

				total_price += tmp.price;
				select_land.push(tmp);
			}
			else break;
		}

		printf("#%d %d\n", tc, Answer);
	}
}