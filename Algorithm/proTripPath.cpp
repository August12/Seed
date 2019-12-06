#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

vector<vector<string>> result;

void dfs(string node, int cnt, int limit, vector<string>& path, map<string,vector<pair<string,bool>>>& ref) {
    if(cnt == limit) {
        result.push_back(path);
        return;
    }
    
    for(int i=0; i<ref[node].size(); i++) {
        if(ref[node][i].second == true) continue;
        ref[node][i].second = true;
        path.push_back(ref[node][i].first);
        dfs(ref[node][i].first, cnt+1, limit, path, ref);
        path.pop_back();
        ref[node][i].second = false;
    }
}

vector<string> solution(vector<vector<string>> tickets) {
    vector<string> answer;
    
    map<string,vector<pair<string,bool>>> m;
    for(int i=0; i<tickets.size(); i++) {
        m[tickets[i][0]].push_back(make_pair(tickets[i][1],false));
    }
    
    int max_size = tickets.size()+1;
    vector<string> path;
    path.push_back("ICN");
    dfs("ICN", 1, max_size, path, m);
    
    sort(result.begin(),result.end());
    answer = result[0];
    return answer;
}