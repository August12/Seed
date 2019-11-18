#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

bool solution(vector<string> phone_book) {
    bool answer = true;
    unordered_map <string, int> umap;
    
    // rehash
    for(string elem : phone_book) {
        umap[elem] = 1;
    }
    
    for(string elem : phone_book) {
        string number = "";
        for(int j=0; j<elem.length(); j++) {
            number += elem[j];
            if(umap[number] && number != elem) {
                answer = false;
                break;
            }
        }
    }
    return answer;
}