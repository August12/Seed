#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int N, M;
int MAP[9][9];
int VIT[9][9][8];
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1};
int Answer = 1e9;

vector < pair< int,pair<int,int> > > v;

void expand(int y, int x, int direct, int plus, int level) {
	int ny = y + dy[direct];
	int nx = x + dx[direct];

	if(ny>=1 && ny<=N && nx>=1 && nx<=M) {
		if(plus == 1) {
			if(MAP[ny][nx] != 6) {
				if(MAP[ny][nx] == 0) {
					MAP[ny][nx] = 7;
					VIT[ny][nx][level] = 1;
				}
				expand(ny,nx,direct,1,level);
			}
		}
		else {
			if(MAP[ny][nx] != 6) {
				VIT[ny][nx][level] = 0;
				if(MAP[ny][nx] == 7) {
					MAP[ny][nx] = 0;
					for(int i=level-1; i>=0; i--) {
						if(VIT[ny][nx][i] == 1) {
							MAP[ny][nx] = 7;	
							break;
						}
					}
				}
				expand(ny,nx,direct,0,level);
			}
		}
	}
}

void check() {
	int sum = 0;
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			if(MAP[r][c] == 0) {
				sum++;
			}
		}
	}

	Answer = Answer > sum ? sum : Answer;
}

void dfs(int cctv) {
	if(cctv == v.size()) {
		printf("\n");
		for(int r=1; r<=N; r++) {
			for(int c=1; c<=M; c++) {
				printf("%d ", MAP[r][c]);
			}
			printf("\n");
		}
		check();
	}
	else {
		if(v[cctv].first == 1) {
			for(int i=0; i<4; i++) {
				expand(v[cctv].second.first,v[cctv].second.second,i,1,cctv);
				dfs(cctv+1);
				expand(v[cctv].second.first,v[cctv].second.second,i,0,cctv);
			}
		}
		else if(v[cctv].first == 2) {
			for(int i=0; i<2; i++) {
				if(i==0) {
					expand(v[cctv].second.first,v[cctv].second.second,0,1,cctv);
					expand(v[cctv].second.first,v[cctv].second.second,2,1,cctv);
					dfs(cctv+1);
					expand(v[cctv].second.first,v[cctv].second.second,0,0,cctv);
					expand(v[cctv].second.first,v[cctv].second.second,2,0,cctv);
				}
				else {
					expand(v[cctv].second.first,v[cctv].second.second,1,1,cctv);
					expand(v[cctv].second.first,v[cctv].second.second,3,1,cctv);
					dfs(cctv+1);
					expand(v[cctv].second.first,v[cctv].second.second,1,0,cctv);
					expand(v[cctv].second.first,v[cctv].second.second,3,0,cctv);
				}
			}
		}
		else if(v[cctv].first == 3) {
			for(int i=0; i<4; i++) {
				if(i==0) {
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,j,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,j,0,cctv);
					}
				}
				else if(i==1) {
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+1)%4,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+1)%4,0,cctv);
					}
				}
				else if(i==2) {
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+2)%4,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+2)%4,0,cctv);
					}
				}
				else {
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+3)%4,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<2; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+3)%4,0,cctv);
					}	
				}
			}
		}
		else if(v[cctv].first == 4) {
			for(int i=0; i<4; i++) {
				if(i==0) {
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,j,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,j,0,cctv);
					}
				}
				else if(i==1) {
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+1)%4,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+1)%4,0,cctv);
					}
				}
				else if(i==2) {
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+2)%4,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+2)%4,0,cctv);
					}
				}
				else {
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+3)%4,1,cctv);
					}
					dfs(cctv+1);
					for(int j=0; j<3; j++) {
						expand(v[cctv].second.first,v[cctv].second.second,(j+3)%4,0,cctv);
					}	
				}
			}
		}
		else {
			for(int i=0; i<4; i++) {
				expand(v[cctv].second.first,v[cctv].second.second,i,1,cctv);
			}
			dfs(cctv+1);
			for(int i=0; i<4; i++) {
				expand(v[cctv].second.first,v[cctv].second.second,i,0,cctv);
			}
		}
	}
}

int main() {
	scanf("%d %d", &N, &M);
	for(int r=1; r<=N; r++) {
		for(int c=1; c<=M; c++) {
			scanf("%d", &MAP[r][c]);
			if(MAP[r][c]>=1 && MAP[r][c]<=5) {
				v.push_back(make_pair(MAP[r][c],make_pair(r,c)));
			}
		}
	}
	
	sort(v.begin(),v.end());
	dfs(0);

	printf("%d\n", Answer);
}