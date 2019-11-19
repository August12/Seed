#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

queue <pair<int,int> > q;
int prior[10];

int solution(vector<int> priorities, int location) {
    int answer = 0;
    
    for(int i=0; i<priorities.size(); i++) {
        q.push(make_pair(i,priorities[i]));
        prior[priorities[i]]++;
    }
    
    while(!q.empty()) {
        int curLoc = q.front().first;
        int curPri = q.front().second;
        q.pop();
        
        bool flag = true;
        for(int i=curPri+1; i<10; i++) {
            if(prior[i] != 0) {
                q.push(make_pair(curLoc,curPri));
                flag = false;
                break;
            }
        }
        
        if(flag) {
            answer++;
            prior[curPri]--;
            if(curLoc == location) break;   
        }
    }
    return answer;
}