#include <stdio.h>
#include <queue>

using namespace std;

int T, N, M, K;
int Map[101][101];
int Answer;

queue < pair<int,char> > q;
queue < pair<int,int> > m;

typedef struct snake_head {
	int direct;
	int y;
	int x;
} Snake_Head;

typedef struct snake_tail {
	int y;
	int x;
} Snake_Tail;

int main() {
	scanf("%d", &T);
	for(int test_case=1; test_case<=T; test_case++) {
		scanf("%d %d %d", &N, &M, &K);
		for(int row=1; row<=N; row++) {
			for(int col=1; col<=N; col++) {
				Map[row][col] = 0;
			}
		}
		while(!q.empty()) {
			q.pop();
		}
		while(!m.empty()) {
			m.pop();
		}

		for(int apple=1; apple<=M; apple++) {
			int row, col;
			scanf("%d %d", &row, &col);
			Map[row][col] = 1;
		}
		for(int i=1; i<=K; i++) {
			int time;
			char direction;
			scanf("%d %c", &time, &direction);
			q.push(make_pair(time, direction));
		}
		Answer = 0;

		Snake_Head s;
		Snake_Tail st;
		s.direct = 0;
		s.y = 1;
		s.x = 1;
		st.y = 1;
		st.x = 1;
		Map[s.y][s.x] = 2;
		while(!q.empty()) {
			printf("AA\n");
			int t = q.front().first;
			char d = q.front().second;
			q.pop();

			for(int count=1; count<=t; count++) {
				
				if(s.direct == 0 && s.x+1 <= N) {
					
					if(Map[s.y][s.x+1] == 1) {
						s.y = s.y;
						s.x = s.x+1;
						Map[s.y][s.x] = 2;
						Answer++;
						m.push(make_pair(s.y,s.x));
					}
					else if(Map[s.y][s.x+1] == 0) {
						
						s.y = s.y;
						s.x = s.x+1;
						Map[s.y][s.x] = 2;
						Map[st.y][st.x] = 0;
						m.push(make_pair(s.y,s.x));
						st.y = m.front().first;
						st.x = m.front().second;
						m.pop();
						Answer++;
					}
					else {
						Answer++;
						printf("#%d %d\n", test_case, Answer);
						return 0;
					}
				}
				else if(s.direct == 1 && s.y+1 <= N) {
					if(Map[s.y+1][s.x] == 1) {
						s.y = s.y+1;
						s.x = s.x;
						Map[s.y][s.x] = 2;
						Answer++;
						m.push(make_pair(s.y,s.x));
					}
					else if(Map[s.y+1][s.x] == 0) {
						s.y = s.y+1;
						s.x = s.x;
						Map[s.y][s.x] = 2;
						Map[st.y][st.x] = 0;
						m.push(make_pair(s.y,s.x));
						st.y = m.front().first;
						st.x = m.front().second;
						m.pop();
						Answer++;
					}
					else {
						Answer++;
						printf("#%d %d\n", test_case, Answer);
						return 0;
					}
				}
				else if(s.direct == 2 && s.x-1 >= 1) {
					if(Map[s.y][s.x-1] == 1) {
						s.y = s.y;
						s.x = s.x-1;
						Map[s.y][s.x] = 2;
						Answer++;
						m.push(make_pair(s.y,s.x));
					}
					else if(Map[s.y][s.x-1] == 0) {
						s.y = s.y;
						s.x = s.x-1;
						Map[s.y][s.x] = 2;
						Map[st.y][st.x] = 0;
						m.push(make_pair(s.y,s.x));
						st.y = m.front().first;
						st.x = m.front().second;
						m.pop();
						Answer++;
					}
					else {
						Answer++;
						printf("#%d %d\n", test_case, Answer);
						return 0;
					}
				}
				else if(s.direct == 3 && s.y-1 >= 1) {
					if(Map[s.y-1][s.x] == 1) {
						s.y = s.y-1;
						s.x = s.x;
						Map[s.y][s.x] = 2;
						Answer++;
						m.push(make_pair(s.y,s.x));
					}
					else if(Map[s.y-1][s.x] == 0) {
						s.y = s.y-1;
						s.x = s.x;
						Map[s.y][s.x] = 2;
						Map[st.y][st.x] = 0;
						m.push(make_pair(s.y,s.x));
						st.y = m.front().first;
						st.x = m.front().second;
						m.pop();
						Answer++;
					}
					else {
						Answer++;
						printf("#%d %d\n", test_case, Answer);
						return 0;
					}
				}
				else {
					continue;
				}
			}

			if(d == 'D') {
				s.direct = (s.direct + 1)%4;
			}
			else {
				if(s.direct-1 == -1) {
					s.direct = 3;
				}
				else {
					s.direct = s.direct - 1;
				}
			}
		}
		printf("#%d %d\n", test_case, Answer);
	}
}