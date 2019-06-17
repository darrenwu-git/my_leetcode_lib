#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//http://alrightchiu.github.io/SecondRound/comparison-sort-heap-sortdui-ji-pai-xu-fa.html

void swap( int *a, int *b) {
    if ( a != b ) {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

void Heapify(int *a, int start, int len) {
    if ( start*2+2 > len-1) return;

    int maxi;
    if(a[start*2+2] > a[start*2+1])
        maxi=start*2+2;
    else
        maxi=start*2+1;

    if(a[start] < a[maxi]) {
        swap(&a[start], &a[maxi]);
        Heapify(a, maxi, len);
    }
}
void heapSort(int *a , int len) {
    for( int i=len/2 ; i>=0 ; i--)
        Heapify(a, i, len);

    while(len>3) {
        swap(&a[0], &a[--len]);
        Heapify(a, 0, len);
    }

    if (len == 3)
        swap(&a[0], &a[2]);
    if (a[0] > a[1])
        swap(&a[0], &a[1]);
}

int main() {
    int b[] = { 2, 5, 6, 32, 3, 9, 87, 8, 7, 1, 0 };
    int len = sizeof(b)/sizeof(b[0]);


    heapSort(b, len);
    for ( int i=0; i<len ; i++ ){
        printf("%d \n", b[i]);
    }
    printf("\n");

    return 0;
}
