#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

bool vit[51];

int solution(string begin, string target, vector<string> words) {
    int answer = 0;
    queue <pair<string, int>> q;
    q.push(make_pair(begin,0));
    while(!q.empty()) {
        string str = q.front().first;
        int level = q.front().second;
        q.pop();
        
        if(str == target) {
            answer = level;
            break;
        }
        
        for(int i=0; i<words.size(); i++) {
            if(vit[i] == true) continue;
            
            int cnt = 0;
            for(int j=0; j<str.size(); j++) {
                if(words[i][j] != str[j]) cnt++;
                if(cnt >= 2) break;
            }
            
            if(cnt == 1) {
                q.push(make_pair(words[i],level+1));
                vit[i] = true;
            }
        }
    }
    return answer;
}