#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    if( a != b ) {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

void quicksort(int *a, int left, int right) {
    int index=left;

    if(left >= right) return;

    for(int i=left+1; i<=right; i++) {
        if(a[i] < a[left])
            swap(&a[i], &a[++index]);
    }
    swap(&a[index], &a[left]);
    quicksort(a, left, index-1);
    quicksort(a, index+1, right);
}

int main() {
    int b[] = { 2,6, 5, 8, 32, 3, 9, 87, 8, 7, 1, 0 };
    int len = sizeof(b)/sizeof(b[0]);
    int *a=malloc(sizeof(int) * len);
    for( int i=0; i< len ; i++)
        a[i] = b[i];

    quicksort(a, 0, len-1);

    for ( int i=0; i<len ; i++ ){
        printf("%d \n", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}

