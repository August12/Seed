#include <stdio.h>

using namespace std;

int N;
int Build[1000001];
int Ans;

void dfs(int start, int end) {
    if(end > N) {
        return;
    }
    else if(Build[start] <= Build[end]) {
        Ans = (Ans < end - start) ? end - start : Ans;
    }
    else {
        dfs(start, end+1);
    }
}

int main(void) {
    scanf("%d",&N);
    for(int i=1; i<=N; i++) {
        scanf("%d",&Build[i]);
    }
    for(int i=1; i<N; i++) {
        dfs(i,i+1);
    }
    
    printf("%d",Ans);
    return 0;
}