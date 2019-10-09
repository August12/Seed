#include <stdio.h>
#include <vector>

using namespace std;

int main() {

	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for(int i=0; i<v.size(); i++) {
		printf("%d\n", v[i]);
	}

	printf("size %lu\n",v.size());
	printf("capacity %lu\n", v.capacity());
	printf("max_size %lu\n", v.max_size());
}