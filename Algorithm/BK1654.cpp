#include <stdio.h>
#include <algorithm>

using namespace std;

long long K, N;
long long L[10000];
long long Cnt, Ans;

void binary_search(long long left, long long right) {
	if(left > right) {
		return;
	}

	long long mid = (left+right)/2;
	Cnt = 0;
	
	for(int i=0; i<K; i++) {
		Cnt += L[i]/mid;
	}

	if(Cnt >= N) {
		Ans = mid;
		binary_search(mid+1,right);
	}
	else {
		binary_search(left,mid-1);
	}
}

int main() {
	scanf("%lld %lld", &K, &N);
	for(int i=0; i<K; i++) {
		scanf("%lld", &L[i]);
	}
	sort(L,L+K);
	long long max = L[K-1];

	binary_search(1,max);
	

	printf("%lld\n", Ans);
}