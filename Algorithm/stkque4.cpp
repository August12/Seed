#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int solution(int bridge_length, int weight, vector<int> truck_weights) {
    int answer = 0;
    int bridge_weight = 0;
    queue< pair<pair<int,int>,int> > bridge_queue;
    queue< int > truck_queue;
    
    for(int i=0; i<truck_weights.size(); i++) {
        truck_queue.push(truck_weights[i]);
    }
    
    while(!truck_queue.empty() || !bridge_queue.empty()) {
        answer++;
        while(!bridge_queue.empty()) {
            int cw = bridge_queue.front().first.first;
            int cp = bridge_queue.front().first.second;
            int ct = bridge_queue.front().second;
            if(ct == answer) break;
            bridge_queue.pop();
            
            cp += 1;
            if(cp == bridge_length+1) {
                bridge_weight -= cw;
                continue;
            }  
            bridge_queue.push(make_pair(make_pair(cw,cp),ct+1));
        }
        if(truck_queue.empty()) continue;
        if(bridge_weight + truck_queue.front() <= weight) {
            bridge_weight += truck_queue.front();
            bridge_queue.push(make_pair(make_pair(truck_queue.front(),1),answer));
            truck_queue.pop();
        }
    }
    return answer;
}