/*
 * @file    rsa.c
 * @author  작성자 이름 / 학번
 * @date    작성 일자
 * @brief   mini RSA implementation code
 * @details 세부 설명
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miniRSA.h"

uint p, q, e, d, n;

uint divide(uint a, uint b) {
	uint tmp=0;

	if(a<b) return 0;
	while(a>=b) {
		a = a-b;
		tmp ++;
	}
	return tmp;
}

uint Mod(uint x, uint n) {
    uint result = 0;

    if(x<n) return x;
    while(x>=n) {
        x = x - n;
        result = x;
    }
    return result;
}

uint gcd(uint a,uint b){
    int temp;
    while(1){
        temp = Mod(a,b);
        // temp = a%h;
        if(temp == 0){
            return b;
        }
        a = b;
        b=temp;
    }
}

/*
 * @brief     모듈러 덧셈 연산을 하는 함수.
 * @param     uint a     : 피연산자1.
 * @param     uint b     : 피연산자2.
 * @param     byte op    : +, - 연산자.
 * @param     uint n      : 모듈러 값.
 * @return    uint result : 피연산자의 덧셈에 대한 모듈러 연산 값. (a op b) mod n
 * @todo      모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
uint ModAdd(uint a, uint b, byte op, uint n) {
    if(n==0) perror("modular is not 0.\n");

    if(op=='-'){
        a = Mod(a,n);
        b = Mod(b,n);
        return (a>b) ? a-b : n-(b-a);
    }else if(op=='+'){
        uint result = a + b;
        if(result<a || result<b)
            return Mod(Mod((-n)+result , n) , n);
        else
            return Mod(result , n);
    }else perror("invalid operation\n");
    return 0;
}

/*
 * @brief      모듈러 곱셈 연산을 하는 함수.
 * @param      uint x       : 피연산자1.
 * @param      uint y       : 피연산자2.
 * @param      uint n       : 모듈러 값.
 * @return     uint result  : 피연산자의 곱셈에 대한 모듈러 연산 값. (a x b) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
uint ModMul(uint x, uint y, uint n) {

    if(n==0) perror("modular is not 0.\n");

    if(x<y){
        x=x^y;
        y=x^y;
        x=x^y;
    }
    uint result;
    for(result=0;y>1;){
        if(y&1){
            result = ModAdd(result,x,'+',n);
            y--;
        }
        else{
            x = ModAdd(x,x,'+',n);
            y>>=1;
        }
    }
    return ModAdd(x,result,'+',n);
}

/*
 * @brief      모듈러 거듭제곱 연산을 하는 함수.
 * @param      uint base   : 피연산자1.
 * @param      uint exp    : 피연산자2.
 * @param      uint n      : 모듈러 값.
 * @return     uint result : 피연산자의 연산에 대한 모듈러 연산 값. (base ^ exp) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
               'square and multiply' 알고리즘을 사용하여 작성한다.
 */
uint ModPow(uint base, uint exp, uint n) {
	uint result = 1;
	base = Mod(base,n);
	
	while(exp>0){
        if(exp&1) result = ModMul(result,base,n);

        base = ModMul(base,base,n);
        exp>>=1;
    }
    return result;
}

/*
 * @brief      입력된 수가 소수인지 입력된 횟수만큼 반복하여 검증하는 함수.
 * @param      uint testNum   : 임의 생성된 홀수.
 * @param      uint repeat    : 판단함수의 반복횟수.
 * @return     uint result    : 판단 결과에 따른 TRUE, FALSE 값.
 * @todo       Miller-Rabin 소수 판별법과 같은 확률적인 방법을 사용하여,
               이론적으로 4N(99.99%) 이상 되는 값을 선택하도록 한다. 
 */

bool witness_prime(uint d, uint n)
{
    int a = ModAdd(2,rand(),'+',n-4);
    uint x = ModPow(a,d,n);
    if(x == 1||x == n-1){
        return TRUE;
    }
    while(d != n-1){
        x = ModMul(x,x,n);
        d *=2;
        if(x == 1){
            return FALSE;
        }
        if(x == n-1){
            return TRUE;
        }
    }
    return FALSE;
}

bool IsPrime(uint testNum, uint repeat) {
	int i;

	if (testNum < 2 || (Mod(testNum,2) == 0 && testNum != 2) || (Mod(testNum,3) == 0 && testNum != 3))
        return FALSE;
    if (testNum <= 3)
        return TRUE;

    uint d = testNum-1;

    while(Mod(d,2)==0){
        d >>= 1;
    }
    for(int i = 0;i<repeat;i++){
        if(witness_prime(d,testNum) == FALSE){

            return FALSE;
        }

    
    }

    return TRUE;
}

/*
 * @brief       모듈러 역 값을 계산하는 함수.
 * @param       uint a      : 피연산자1.
 * @param       uint m      : 모듈러 값.
 * @return      uint result : 피연산자의 모듈러 역수 값.
 * @todo        확장 유클리드 알고리즘을 사용하여 작성하도록 한다.
 */
uint ModInv(uint a, uint m) {
    uint result, u1, u3, v1, v3, t1, t3, q;
  	int iter;
    
    u1 = 1;
    u3 = a;
    v1 = 0;
    v3 = m;
    
    iter = 1;
    
    while (v3 != 0)
    {
        q = divide(u3,v3);
        t3 = Mod(u3,v3);
        t1 = u1 + q * v1;
        
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = t3;
        iter = -iter;
    }
    
    if (u3 != 1)
        perror("No inverse exists");
    
    if (iter < 0) result = m - u1;
    else result = u1;
    return result;
}

/*
 * @brief     RSA 키를 생성하는 함수.
 * @param     uint *p   : 소수 p.
 * @param     uint *q   : 소수 q.
 * @param     uint *e   : 공개키 값.
 * @param     uint *d   : 개인키 값.
 * @param     uint *n   : 모듈러 n 값.
 * @return    void
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
void miniRSAKeygen(uint *p, uint *q, uint *e, uint *d, uint *n) {
    *n = (*p)*(*q);

    int i = 0;
    uint p1 = *p-1;
    uint q1 = *q-1;

    uint theta  = p1*q1;

    srand(time(NULL));
    while(1){
        *e = Mod(rand(),((theta-2)+1)+2);
        if(gcd(*e,theta)==1){
                break;
        }
        
    } 
    *d = ModInv(*e,theta);
}

/*
 * @brief     RSA 암복호화를 진행하는 함수.
 * @param     uint data   : 키 값.
 * @param     uint key    : 키 값.
 * @param     uint n      : 모듈러 n 값.
 * @return    uint result : 암복호화에 결과값
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
uint miniRSA(uint data, uint key, uint n) {
    uint result;
    uint i;
    if(key == e){
        result = ModPow(data,e,n);
    }else if(key == d){
        result = ModPow(data,d,n);
    }
    return result;
}

uint GCD(uint a, uint b) {
    uint prev_a;

    while(b != 0) {
        printf("GCD(%u, %u)\n", a, b);
        prev_a = a;
        a = b;
        while(prev_a >= b) prev_a -= b;
        b = prev_a;
    }
    printf("GCD(%u, %u)\n\n", a, b);
    return a;
}

int main(int argc, char* argv[]) {
    byte plain_text[4] = {0x12, 0x34, 0x56, 0x78};
    uint plain_data, encrpyted_data, decrpyted_data;
    uint seed = time(NULL);

    memcpy(&plain_data, plain_text, 4);

    // 난수 생성기 시드값 설정
    seed = time(NULL);
    InitWELLRNG512a(&seed);
    uint k = 9;
    double a,b;
    while(1){
        a = WELLRNG512a();
        b = WELLRNG512a();   
        p = (uint)(a*RND_MAX);
        q = (uint)(b*RND_MAX);
        if(IsPrime(q,k)==TRUE&&IsPrime(p,k)==TRUE){
            if(p*q>plain_data)
                break;
        }
    }

    // RSA 키 생성
    miniRSAKeygen(&p, &q, &e, &d, &n);
    printf("0. Key generation is Success!\n ");
    printf("p : %u\n q : %u\n e : %u\n d : %u\n N : %u\n\n", p, q, e, d, n);

    // RSA 암호화 테스트
    encrpyted_data = miniRSA(plain_data, e, n);
    printf("1. plain text : %u\n", plain_data);    
    printf("2. encrypted plain text : %u\n\n", encrpyted_data);

    // RSA 복호화 테스트
    decrpyted_data = miniRSA(encrpyted_data, d, n);
    printf("3. cipher text : %u\n", encrpyted_data);
    printf("4. Decrypted plain text : %u\n\n", decrpyted_data);

    // 결과 출력
    printf("RSA Decryption: %s\n", (decrpyted_data == plain_data) ? "SUCCESS!" : "FAILURE!");

    return 0;
}