#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> array, vector<vector<int>> com) {
    vector<int> answer;
    
    for(int c=0; c<com.size(); c++) {
        int i=com[c][0]-1;
        int j=com[c][1]-1;
        int k=com[c][2]-1;
        
        vector<int> tmp;
        for(int n=i; n<=j; n++) {
            tmp.push_back(array[n]);
        }
        
        sort(tmp.begin(),tmp.end());
        
        answer.push_back(tmp[k]);
    }
    
    return answer;
}