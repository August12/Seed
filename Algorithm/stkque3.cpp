#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> heights) {
    vector<int> answer;
    for(int i=0; i<heights.size(); i++) {
        int result = 0;
        for(int j=i-1; j>=0; j--) {
            if(heights[i] < heights[j]) {
                result = j+1;
                break;
            }
        }
        answer.push_back(result);
    }
    return answer;
}