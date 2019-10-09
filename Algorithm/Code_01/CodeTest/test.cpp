#include <stdio.h>
#include <queue>

using namespace std;

int C, B;
int db[3] = {-1, 1, 2};
int Ans;

queue < pair< pair<int,int>, pair<int,int> > > q;

int main(void) {
    scanf("%d %d", &C, &B);
    Ans = -1;
    
    if(C == B) {
        Ans = 0;
        printf("%d",Ans);
        return 0;
    }
    
    q.push(make_pair(make_pair(C,B),make_pair(1,0)));
    while(!q.empty()) {
        int now_C = q.front().first.first;
        int now_B = q.front().first.second;
        int now_dC = q.front().second.first;
        int now_time = q.front().second.second;
        q.pop();
        
        if(now_C == now_B) {
            Ans = now_time;
            printf("%d", now_time);
            return 0;
        }
        
        for(int i=0; i<3; i++) {
            if(i==2) {
                int nxt_C = now_C + now_dC;
                int nxt_B = now_B*db[i];
                
                if(nxt_C>=0 && nxt_C<=200000 && nxt_B>=0 && nxt_B<=200000) {
                    q.push(make_pair(make_pair(nxt_C,nxt_B),make_pair(now_dC+1,now_time+1)));
                }
            }
            else {
                int nxt_C = now_C + now_dC;
                int nxt_B = now_B + db[i];
                
                if(nxt_C>=0 && nxt_C<=200000 && nxt_B>=0 && nxt_B<=200000) {
                    q.push(make_pair(make_pair(nxt_C,nxt_B),make_pair(now_dC+1,now_time+1)));
                }
            }
        }
    }
    printf("%d",Ans);
    return 0;
    
}