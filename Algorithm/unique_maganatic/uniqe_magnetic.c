#include <stdio.h>

int T = 0;
int K = 0;
int m[4][8];
int R[20];
int r[20];

void input() {

	int j,k;

	scanf("%d",&K); // 몇 번 도는지 횟수 받기.

	for(j=0;j<4;j++) {
		for(k=0;k<8;k++) {
			scanf("%d",&m[j][k]); // 자석 마다 정보 입력 받기.
		}
	}

	for(k=0;k<K;k++) { // 몇 번째 바퀴가 시계,반시계 도는 방향 입력 받기.
		scanf("%d",&R[k]);
		scanf("%d",&r[k]);
	}
}
/*
void print_test() {

	int i,j;

	printf("%d\n", K);

	for(i=0;i<4;i++) {
		for(j=0;j<8;j++) {
			printf("%d", m[i][j]);
		}
		printf("\n");
	}

	for(i=0;i<K;i++) {
		printf("%d %d\n", R[i],r[i]);
	}

}
*/


void solve() {

	int i=0,j=0;
	int k,d;

	while(K>0) {

		k = R[i];
		d = r[i];

		if(k==1){

			int flag = 1;

			while(flag) {

				if(k<4)			
					flag = (m[k-1][2] == m[k][6]) ? 0 : 1;
				else
					flag = 0;

				int temp;

				if(d==1){
					temp = m[k-1][7];

					for(j=7;j>0;j--)
						m[k-1][j] = m[k-1][j-1];

					m[k-1][0] = temp;
				}
				else {
					temp = m[k-1][0];

					for(j=0;j<7;j++)
						m[k-1][j] = m[k-1][j+1];

					m[k-1][7] = temp;
				}

				d = (d==1) ? -1 : 1;
				k++;

			}
			
		}
		else if(k==4) {

			int flag = 1;

			while(flag) {

				if(k>1)
					flag = (m[k-1][6] == m[k-2][2]) ? 0 : 1;
				else
					flag = 0;

				int temp;

				if(d==1) {
					temp = m[k-1][7];

					for(j=7;j>0;j--)
						m[k-1][j] = m[k-1][j-1];

					m[k-1][0] = temp;
				}
				else {
					temp = m[k-1][0];

					for(j=0;j<7;j++)
						m[k-1][j] = m[k-1][j+1];

					m[k-1][7] = temp;
				}

				d = (d==1) ? -1 : 1;
				k--;
			}
		}
		else {
			
			int flag_right = 1;
			int flag_left = 1;
			int k2 = k;
			int d2 = d;

			flag_left = (m[k-1][6] == m[k-2][2]) ? 0 : 1;

			while(flag_right) {

				if(k<4)
					flag_right = (m[k-1][2] == m[k][6]) ? 0 : 1;
				else
					flag_right = 0;

				int temp;

				if(d==1){
					temp = m[k-1][7];

					for(j=7;j>0;j--)
						m[k-1][j] = m[k-1][j-1];

					m[k-1][0] = temp;
				}
				else {
					temp = m[k-1][0];

					for(j=0;j<7;j++)
						m[k-1][j] = m[k-1][j+1];

					m[k-1][7] = temp;
					
				}

				d = (d==1) ? -1 : 1;
				k++;
			}

			while(flag_left) {

				d2 = (d2==1) ? -1 : 1;
				k2--;

				if(k2>1)
					flag_left = (m[k2-1][6] == m[k2-2][2]) ? 0 : 1;
				else
					flag_left = 0;

				int temp;

				if(d2==1) {
					temp = m[k2-1][7];

					for(j=7;j>0;j--)
						m[k2-1][j] = m[k2-1][j-1];

					m[k2-1][0] = temp;
				}
				else {
					temp = m[k2-1][0];

					for(j=0;j<7;j++)
						m[k2-1][j] = m[k2-1][j+1];

					m[k2-1][7] = temp;
				}
			}

		}

	K--;
	i++;


	}

}

int main()
{
	int test_case=0,j=0;

	//freopen("sample_input.txt","r",stdin);

	scanf("%d",&T); // 총 테스트 케이스 갯수 받기.
	
	//printf("%d\n", T);

	for(i=0;i<T;i++) {
		
		int sum = 0;

		input();

		//print_test();


		solve();

		for(j=0;j<4;j++) {
			if(j==0)
				sum += (m[0][0]==1) ? 1 : 0;
			else if(j==1)
				sum += (m[1][0]==1) ? 2 : 0;
			else if(j==2)
				sum += (m[2][0]==1) ? 4 : 0;
			else
				sum += (m[3][0]==1) ? 8 : 0;
		
		}


		//print_test();
		printf("#%d %d\n\n", i+1, sum);	

	}	


	return 0;
}