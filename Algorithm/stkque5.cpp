#include <string>
#include <vector>

using namespace std;

int solution(string arrangement) {
    int answer = 0;
    int open = 0;
    string befo = "";
    
    for(int i=0; i<arrangement.length(); i++) {
        if(arrangement[i] == ')' && befo == "(") {
            open--;
            answer += open;
            befo = "";
        }
        else if(arrangement[i] == '(') {
            open++;
            befo = "(";
        }
        else {
            answer += 1;
            open -= 1;
            befo = "";
        }
    }
    return answer;
}