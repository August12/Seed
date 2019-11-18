#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct song {
    int num;
    int play;
}Song;

typedef struct node {
    priority_queue<Song> pq;
    long long sum;
}Node;

bool operator<(Song a, Song b) {
    if (a.play == b.play) return a.num > b.num;
    else return a.play < b.play;
}

bool comp(pair<string,Node> a, pair<string,Node> b) {
    return a.second.sum > b.second.sum;
}

vector<int> solution(vector<string> genres, vector<int> plays) {
    vector<int> answer;
    vector<pair<string,Node> > v;
    map<string,Node> m;
    
    // rehash
    for(int i=0; i<plays.size(); i++) {
        if(m.find(genres[i]) == m.end()) {
            Song s;
            s.num = i;
            s.play = plays[i];
            
            m[genres[i]].pq.push(s);
            m[genres[i]].sum = plays[i];
        }
        else {
            Song s;
            s.num = i;
            s.play = plays[i];
            
            m[genres[i]].pq.push(s);
            m[genres[i]].sum += plays[i];
        }
    }
    
    // map to vector for sorting
    for(auto it = m.begin(); it != m.end(); it++) {
        v.push_back(make_pair(it->first,it->second));
    }
    sort(v.begin(),v.end(),comp);
    
    // solution
    for(int i=0; i<v.size(); i++) {
        for(int j=0; j<2; j++) {
            if(v[i].second.pq.empty()) break;
            
            answer.push_back(v[i].second.pq.top().num);
            v[i].second.pq.pop();
        }
    }
    return answer;
}