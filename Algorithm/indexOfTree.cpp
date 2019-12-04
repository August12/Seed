#include <stdio.h>

using namespace std;

char arr[10] = {'A', 'B', 'C', 'D','E', 'F', 'G', 'H', 'I', 'J'};
int arrlen = sizeof(arr)/sizeof(*arr);

void postorder(int root_idx) {
    if(2*root_idx + 1 < arrlen) postorder(2*root_idx + 1);
    if(2*root_idx + 2 < arrlen) postorder(2*root_idx + 2);
    printf("%c ", arr[root_idx]);
}

void preorder(int root_idx) {
    printf("%c ", arr[root_idx]);
    if(2*root_idx + 1 < arrlen) preorder(2*root_idx + 1);
    if(2*root_idx + 2 < arrlen) preorder(2*root_idx + 2);
}

void inorder(int root_idx) {
    if(2*root_idx + 1 < arrlen) inorder(2*root_idx + 1);
    printf("%c ", arr[root_idx]);
    if(2*root_idx + 2 < arrlen) inorder(2*root_idx + 2);
}

int main(void) {

    printf("postorder : ");
    postorder(0);
    printf("\n");

    printf("preorder : ");
    preorder(0);
    printf("\n");

    printf("inorder : ");
    inorder(0);
    printf("\n");
}