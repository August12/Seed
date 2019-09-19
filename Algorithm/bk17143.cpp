#include <stdio.h>

int R, C, M;
int ANSWER;
int dir[4] = {1,2,3,4};

typedef struct {
	int S;
	int D;
	int Z;
}SHARK;

SHARK MAP[102][102];

void move_shark(void);
void fishing(int col);

int main(void) {
	scanf("%d %d %d %d", &R, &C, &M);
	for(int i=0; i<M; i++) {
		int r, c, s, d, z;
		scanf("%d %d %d %d %d", &r, &c, &s, &d, &z);
		MAP[r][c].S = s;
		MAP[r][c].D = d;
		MAP[r][c].Z = z;
	}

	for(int col=1; col<=C; col++) {
		fishing(col);
		move_shark();
	}

	printf("%d\n", ANSWER);
}

void fishing(int col) {
	for(int r=1; r<=R; r++) {
		if(MAP[r][col].Z != 0) {
			ANSWER += MAP[r][col].Z;
			MAP[r][col].Z = 0;
		}
	}
}

void move_shark(void) {
	for(int r=1; r<=R; r++) {
		for(int c=1; c<=C; c++) {
			if(MAP[r][c].Z != 0) {
				
				SHARK shark = MAP[r][c];
				
				if(shark.D == 1) {
					int s = shark.S % (R-1);
				}
			}
		}
	}
}