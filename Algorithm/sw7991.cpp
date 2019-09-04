#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int T;
int N;
vector < vector<int> > student;
vector < int > line;

int main(void) {
	scanf("%d", &T);

	for(int tc=1; tc<=T; ++tc) {
		scanf("%d", &N);

		int ptmp = 0, ntmp = 0;
		int pidx = 0, nidx = 0;
		for(int i=0; i<N; ++i) {
			scanf("%d", &ntmp);

			if(ptmp == ntmp) student[pidx].push_back(ntmp);
			else {
				vector <int> v;

				student.push_back(v);
				student[nidx].push_back(ntmp);
				pidx = nidx;
				nidx++;
				ptmp = ntmp;
			}
		}

		sort(student.begin(),student.end());

		int idx = 0, min_idx = 0;
		for(int count=1; count<=N; ++count) {
			if(count == 1) {
				line.push_back(student[idx][0]);
				if(idx == min_idx && student[idx].size() > 1) {
					for(int i=1; i<student[idx].size(); ++i) line.push_back(student[idx][i]);
				}
				idx++;	
			}
			else {
				if(student[idx][0] != student[idx-1][0] + 1) {
					
				}
			}
		}

		// for(int i=0; i<student.size(); ++i) {
		// 	for(int j=0; j<student[i].size(); ++j) {
		// 		printf("%d ", student[i][j]);
		// 	}
		// 	printf("\n");
		// }
	}

}