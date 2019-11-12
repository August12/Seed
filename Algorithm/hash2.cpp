#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

vector<string> phone_book = { "119", "120", "1191" };

bool solution(vector<string> phone_book) {
    bool answer = true;
    sort(phone_book.begin(),phone_book.end());
    for(int i=0; i<phone_book.size()-1; i++) {
        string cur = phone_book[i];
        string nxt = phone_book[i+1];
        
        if(cur.length() > nxt.length()) continue;
        
        if(cur == nxt.substr(0,cur.length())) {
            answer = false;
            break;
        }
    }
    return answer;
}

int main(void) {
	printf(solution(phone_book) ? "true" : "false");
	printf("\n");
}