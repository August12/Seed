#include <iostream>
#include <string>
#include <queue>
using namespace std;

const int Max = 1000;

int dy[4] = {0, 0, 1, -1};
int dx[4] = {1, -1, 0, 0};

int N, M;
int Map[Max][Max];
int Visit[Max][Max][2]; // 벽 뚫기 여부

int BFS(void) {
	queue < pair<pair<int,int>,int> > q;
	q.push(make_pair(make_pair(0,0),0));
	Visit[0][0][0] = 1;

	while(!q.empty()) {
		int y = q.front().first.first;
		int x = q.front().first.second;
		int f = q.front().second;
		q.pop();
		// cout << "pop node : " << y << x << endl;

		// 도착하면
		if(y == N-1 && x == M-1)
			return Visit[y][x][f];

		for(int i = 0;i < 4;i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];

			if(ny >= 0 && ny < N && nx >= 0 && nx < M) {
				if(Map[ny][nx] == 1 && f == 0) {
					Visit[ny][nx][f+1] = Visit[y][x][f] + 1;
					q.push(make_pair(make_pair(ny,nx),f+1));
				}
				else if(Map[ny][nx] == 0 && Visit[ny][nx][f] == 0) {
					Visit[ny][nx][f] = Visit[y][x][f] + 1;
					q.push(make_pair(make_pair(ny,nx),f));
				}
			}
		}
	}

	return -1;
}

int main(void)
{
	cin >> N >> M;

	for(int i=0;i<N;i++)
	{
		string tmp;
		cin >> tmp;

		for(int j=0;j<M;j++)
			Map[i][j] = tmp[j] - '0';
	}
	
	cout << BFS() << endl;
	return 0;
}