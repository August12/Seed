#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

string solution(vector<string> participant, vector<string> completion) {
	string answer = "";
	unordered_map<string, int> map;

	// rehash
	for(string elem : completion) {
		// if elem not in map
		if(map.end() == map.find(elem)) {
			map[elem] = 1;
		}
		else {
			map[elem]++;
		}
	}

	for(string elem : participant) {
		if(map.end() == map.find(elem)) {
			answer = elem;
			break;
		}
		else {
			map[elem]--;
			if(map[elem] < 0) {
				answer = elem;
				break;
			}
		}
	}

	return answer;
}

int main(void) {
	vector <string> participant = { "marina", "josipa", "nikola", "vinko", "filipa"};
	vector <string> completion = { "josipa", "filipa", "marina", "nikola" };
	printf("%s\n",solution(participant,completion).c_str());
}