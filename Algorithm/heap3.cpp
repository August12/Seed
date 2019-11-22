#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int solution(vector<vector<int>> jobs) {
    int answer = 0, end = 0, time = 0, idx = 0;
    bool working = false;
    vector<int> ret;
    priority_queue< pair<int, int>, vector< pair<int,int> >, greater< pair<int,int> > > pq;

    sort(jobs.begin(), jobs.end());
    int size = jobs.size();
    while (true) {
        if (idx == size && pq.empty()) break;
        if (time == end) working = false;
        if (idx != size) {
            while (time == jobs[idx][0]) {
                pq.push(make_pair(jobs[idx][1], jobs[idx][0]));
                idx++;
                if (idx == size) break;
            }
        }
        if (!working && !pq.empty()) {
            working = true;
            end = time + pq.top().first;
            ret.push_back(end - pq.top().second);
            pq.pop();
        }
        time++;

    }

    for (int i = 0; i < ret.size(); i++) {
        answer += ret[i];
    }
    answer /= ret.size();

    return answer;
}