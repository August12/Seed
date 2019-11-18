#include <string>
#include <vector>
#include <map>

using namespace std;

vector<vector<string> > clothes = {{"yellow_hat", "headgear"}, {"blue_sunglasses", "eyewear"}, {"green_turban", "headgear"}};

int solution(vector<vector<string>> clothes) {
    int answer = 1;
    map<string, vector<string>> m;
    
    // rehash
    for(int i=0; i<clothes.size(); i++) {
        m[clothes[i][1]].push_back(clothes[i][0]);
    }
    
    map<string, vector<string>>::iterator itr;
    for(itr=m.begin(); itr!=m.end(); itr++) {
        answer *= m[itr->first].size()+1;
    }
    
    return answer-1;
}

int main(void) {
	int num = solution(clothes);
}