#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int solution(int stock, vector<int> dates, vector<int> supplies, int k) {
    int answer = 0;
    priority_queue< int > pq;
    queue< pair<int,int> > q;
    
    for(int i=0; i<dates.size(); i++) {
        q.push(make_pair(dates[i],i));
    }
    
    for(int i=0; i<k; i++) {
        if(i == q.front().first) {
            pq.push(supplies[q.front().second]);
            q.pop();
        }
        
        if(stock == 0) {
            stock += pq.top();
            pq.pop();
            answer++;
        }
        
        stock--;
    }
    return answer;
}