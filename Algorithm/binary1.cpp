#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> budgets, int M) {
    int answer = 0;
    long long tot_sum = 0;
    long long left = 1, right = M;
    
    sort(budgets.begin(),budgets.end());
    for(int i=0; i<budgets.size(); i++) {
        tot_sum += budgets[i];
    }
    
    if(tot_sum <= M) return budgets[budgets.size()-1];
    
    long long tmp = 0;
    while(true) {
        long long mid = (left+right)/2;
        long long sum = 0;
        
        if(mid == tmp) return mid;
        
        for(int i=0; i<budgets.size(); i++) {
            if(budgets[i] > mid) sum += mid;
            else sum += budgets[i];
        }
        
        if(sum == M) {
            return mid;
        }
        else if(sum < M) {
            left = mid;
        }
        else {
            right = mid;
        }
        tmp = mid;
    }
    
    return answer;
}