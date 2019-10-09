#include <stdio.h>

int T, N;
char wall[1001];
char direct[1001];

typedef struct cube {
	char f[3][3];
	char b[3][3];
	char u[3][3];
	char d[3][3];
	char l[3][3];
	char r[3][3];
} Cube;

Cube c;

c.f = {
				{'r', 'r', 'r'},
				{'r', 'r', 'r'},
				{'r', 'r', 'r'}
};
c.b = {
				{'o', 'o', 'o'},
				{'o', 'o', 'o'},
				{'o', 'o', 'o'}
};
c.u = {

				{'w','w','w'},
				{'w','w','w'},
				{'w','w','w'}
};
c.d = {
				{'y','y','y'},
				{'y','y','y'},
				{'y','y','y'}
};
c.l = {
				{'g','g','g'},
				{'g','g','g'},
				{'g','g','g'}
};
c.r = {
				{'b','b','b'},
				{'b','b','b'},
				{'b','b','b'}
};

int main() {
	// scanf("%d", &T);
	// for(int test_case=1; test_case<=T; test_case++) {
	// 	for(int n=1; n<=N; n++) {
	// 		scanf("%s %s", &wall[n], &direct[n]);

	// 		char tmp[3];
	// 		if(wall[n] == 'L') {
	// 			if(direct[n] == '-') {
	// 				for(int i=0; i<3; i++) {
	// 					tmp[i] = c.f[i][0];
	// 					c.f[i][0] = c.d[i][0];
	// 					c.d[i][0] = c.b[i][0];
	// 					c.b[i][0] = c.u[i][0];
	// 					c.u[i][0] = tmp[i];
	// 				}
	// 			}
	// 			else {
	// 				for(int i=0; i<3; i++) {
	// 					tmp[i] = c.f[i][0];
	// 					c.f[i][0] = c.u[i][0];
	// 					c.u[i][0] = c.b[i][0];
	// 					c.b[i][0] = c.d[i][0];
	// 					c.d[i][0] = tmp[i];
	// 				}

	// 			}
	// 		}
	// 		else if (wall[n] == 'R') {
	// 			if(direct[n] == '-') {
	// 				for(int i=0; i<3; i++) {
	// 					tmp[i] = c.f[i][2];
	// 					c.f[i][2] = c.u[i][2];
	// 					c.u[i][2] = c.b[i][2];
	// 					c.b[i][2] = c.d[i][2];
	// 					c.d[i][2] = tmp[i];
	// 				}
	// 			}
	// 			else {
	// 				for(int i=0; i<3; i++) {
	// 					tmp[i] = c.f[i][2];
	// 					c.f[i][2] = c.d[i][2];
	// 					c.d[i][2] = c.b[i][2];
	// 					c.b[i][2] = c.u[i][2];
	// 					c.u[i][2] = tmp[i];
	// 				}

	// 			}
	// 		}




			// else if (wall[n] == 'F') {
			// 	if(direct[n] == '-') {
			// 		for(int i=0; i<3; i++) {
			// 			tmp[i] = c.l[i][2];
			// 			c.l[i][2] = c.f[2][(i+1)2];

			// 		}
			// 	}
			// 	else {
			// 		for(int i=0; i<3; i++) {
						
			// 		}

			// 	}
			// }
		}
	}

	for(int r=0; r<3; r++) {
		for(int c=0; c<3; c++) {
			printf("%s", c.u[r][c]);
		}
		printf("\n");
	}
}