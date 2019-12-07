#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char* solution(int a, int b) {
    const char* days[] = {"FRI","SAT","SUN","MON","TUE","WED","THU"};
    int month[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int cnt = 0;
    
    for(int i=0; i<a-1; i++) {
        cnt += month[i];
    }
    cnt += b-1;
    
    char* answer = (char*)malloc(sizeof(days)); 
    answer = days[cnt%7];
    return answer;
}