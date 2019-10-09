#include <string>
#include <iostream>
using namespace std;

int T;
int ans[3];
int a[2][100001];

int main(void) {
	scanf("%d", &T);

	for(int tc=1; tc<=T; tc++) {
		// 초기화.
		ans[0] = ans[1] = ans[2] = 0;
		
		int n=10, k=20;
		string t;
		cin.ignore();
		getline(cin,t);

		printf("%s\n", t.c_str());

		int cnt = 0;
		// 초기 string 값 배열에 할당.
		for(int i=1; i<=n; i++) {
			if(t[i] == 'R') a[cnt][i] = 2;
			else if(t[i] == 'G') a[cnt][i] = 1;
			else a[cnt][i] = 0;
		}

		/*****************/
		while(k > 0) {
			cnt = 1 - cnt;
			for(int i = 0; i < n; i++){
                a[cnt][i] = (a[1 - cnt][i] + a[1 - cnt][(i + 1) % n] + a[1 - cnt][(i + n - 1) % n]) % 3;
            }


		}

		/*****************/
	}
}

/*
#include <cstring>
#include <iostream>
using namespace std;
 
int T;
int ans[3];
int a[2][100001];
int main(void) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        ans[0] = ans[1] = ans[2] = 0;
        int n, k;
        cin >> n >> k;
        string t;
        cin >> t;
        int cnt = 0;
        for(int i = 0; i < n; i++){
            if(t[i] == 'R') a[cnt][i] = 2;
            else if(t[i] == 'G') a[cnt][i] = 1;
            else a[cnt][i] = 0;
        }
        while(k > 0){
            int r = 1;
            while(r * 3 < k) r *= 3;
            k -= r;
            cnt = 1 - cnt;
            for(int i = 0; i < n; i++){
                a[cnt][i] = (a[1 - cnt][i] + a[1 - cnt][(i + r) % n] + a[1 - cnt][(i + r * 2) % n]) % 3;
            }
        }
        for(int i = 0; i < n; i++)
            ans[a[cnt][i]]++;
        cout << '#' << tc << ' ' << ans[2] << ' '<< ans[1] << ' ' << ans[0] << '\n';
    }
    return 0;
}
*/