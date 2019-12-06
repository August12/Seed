#include <string>
#include <vector>
#include <cstring>
using namespace std;

void dfs(vector<vector<int>> map, int chk[], int node) {
    if(chk[node] == 1) return;
    chk[node] = 1;
    for(int i=0; i<map[node].size(); i++) {
        if(node == i) continue;
        if(map[node][i] == 0) continue;
        dfs(map, chk, i);
    }
}

int solution(int n, vector<vector<int>> computers) {
    int answer = 0;
    int visit[200];
    memset(visit,0,sizeof(visit));
    
    for(int i=0; i<computers.size(); i++) {
        if(visit[i] == 1) continue;
        answer++;
        dfs(computers, visit, i);
    }
    return answer;
}