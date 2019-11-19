#include <string>
#include <vector>

using namespace std;

bool chk[100];

vector<int> solution(vector<int> progresses, vector<int> speeds) {
    vector<int> answer;
    for(int i=0; i<progresses.size(); i++) {
        if(chk[i]) continue;
        
        int cnt = 1;
        int remain = 100 - progresses[i];
        int day = remain / speeds[i];
        chk[i] = true;
        if(remain % speeds[i] != 0) day++;
        
        bool flag = true;
        for(int j=i+1; j<progresses.size(); j++) {
            if(flag) {
                if(!chk[j] && progresses[j]+speeds[j]*day>=100) {
                    cnt++;
                    chk[j] = true;
                }
                else {
                    flag = false;
                    progresses[j] += speeds[j]*day;
                }
            }
            else {
                progresses[j] += speeds[j]*day;
            }
        }
        
        answer.push_back(cnt);
    }
    return answer;
}