#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<int> solution(vector<string> oper) {
    vector<int> answer;
    deque <int> dq;
    
    for(int i=0; i<oper.size(); i++) {
        if(oper[i][0] == 'I') {
            int value = stoi(oper[i].substr(2,oper[i].length()));
            dq.push_back(value);
            sort(dq.begin(),dq.end());
        }
        else {
            if(oper[i].substr(2,oper[i].length()) == "1") {
                if(dq.empty()) continue;
                dq.pop_back();
            }
            else {
                if(dq.empty()) continue;
                dq.pop_front();
            }
        }
    }
    if(dq.empty()) {
        answer.push_back(0);
        answer.push_back(0);
    }
    else {
        answer.push_back(dq.back());
        answer.push_back(dq.front());
    }
    
    return answer;
}