#include <string>
#include <vector>
#include <queue>

using namespace std;

int bl = 2;
int w = 10;
vector<int> t_w = {7,4,5,6};

int solution(int bridge_length, int weight, vector<int> truck_weights) {
    
    int time = 0;
    int bw = 0;
    queue<int> twq;
    queue<int> bwq;
    
    for(int i=0; i<truck_weights.size(); i++) {
        twq.push(truck_weights[i]);
    }
    
    while(!twq.empty() || !bwq.empty()) {
        time++;
        if(time%(bridge_length+1) == 0) {
            if(!bwq.empty()) {
                bw -= bwq.front();
                bwq.pop();
            }
        }
        
        if(bw + twq.front() <= weight) {
            bw += twq.front();
            bwq.push(twq.front());
            twq.pop();
        }
        
    }
    return time;
}

int main(void) {
    int answer = 0;
    answer = solution(bl,w,t_w);
    printf("answer:%d\n", answer);
}