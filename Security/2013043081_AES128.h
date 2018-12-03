
// 암호화 모드
#define ENC 1 
// 복호화 모드
#define DEC 0 

typedef unsigned char BYTE;

void expandKey(BYTE *key, BYTE *roundKey);
BYTE* subBytes(BYTE *state, int mode);
BYTE* shiftRows(BYTE *state, int mode);
BYTE* mixColumns(BYTE *state, int mode);
BYTE* addRoundKey(BYTE *state, BYTE *rKey);

// 128비트 AES 암복호화 인터페이스
void AES128(BYTE *plain, BYTE *cipher, BYTE *key, int mode);