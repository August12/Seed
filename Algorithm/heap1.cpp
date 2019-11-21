#include <string>
#include <vector>
#include <queue>

using namespace std;

int solution(vector<int> s, int K) {
    int answer = 0;
    priority_queue < int,vector<int>,greater<int> > pq;
    for(int i=0; i<s.size(); i++) {
        pq.push(s[i]);
    }
    
    while(pq.top() < K) {
        if(pq.size() < 2) return -1;
        int target = pq.top();
        pq.pop();
        target += (pq.top()*2);
        pq.pop();
        pq.push(target);
        answer++;
    }
    return answer;
}