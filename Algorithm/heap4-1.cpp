#include <string>
#include <vector>
#include <queue>

using namespace std;

vector<int> solution(vector<string> oper) {
    vector<int> answer;
    priority_queue < int > maxH;
    priority_queue < int, vector<int>, greater<int> > minH;
    
    for(int i=0; i<oper.size(); i++) {
        if(oper[i][0] == 'I') {
            int value = stoi(oper[i].substr(2,oper[i].length()));
            maxH.push(value);
            minH.push(value);
        }
        else {
            if(oper[i].substr(2,oper[i].length()) == "1") {
                if(maxH.empty()) continue;
                if(maxH.top() == minH.top()) minH.pop();
                maxH.pop();
                
            }
            else {
                if(minH.empty()) continue;
                if(minH.top() == maxH.top()) maxH.pop();
                minH.pop();
                
            }
            
            if(maxH.top() < minH.top()) {
                while(!minH.empty()) minH.pop();
                while(!maxH.empty()) maxH.pop();
            }
        }
    }
    if(maxH.empty()) {
        answer.push_back(0);
        answer.push_back(0);
    }
    else {
        answer.push_back(maxH.top());
        answer.push_back(minH.top());
    }
    
    return answer;
}