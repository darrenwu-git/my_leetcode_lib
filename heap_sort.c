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

void MaxHeapify( int *a, int start, int len ) {
    int maxi;

    if( start*2+1 > len -1 ) return;

    if( a[start *2] > a[start *2 + 1] )
        maxi = start*2;
    else
        maxi = start*2 + 1;

    if( a[start] >= a[maxi] ) return;

    if( a[start] < a[maxi] ) {
        swap(&a[start], &a[maxi]);
        MaxHeapify(a, maxi, len);
    }
}
void buildMaxHeap( int *a, int len ) {

    for( int i=len/2 ;  i>=1 ; i-- ){
        MaxHeapify(a, i, len);
    }
}

int * heapSort(int *a, int len ){
    len = len + 1;
    int *heap=malloc( sizeof(int) * (len));
    for (int i=1; i<len; i++)
        heap[i] = a[i-1];
    
    buildMaxHeap(heap, len);

    while(len > 1) {
        swap( &heap[1], &heap[len-1] );
        len--;
        MaxHeapify(heap, 1, len);
    }

    return heap;
}

int main() {
    int b[] = { 2,6, 5, 8, 32, 3, 9, 87, 8, 7, 1, 0 };
    int len = sizeof(b)/sizeof(b[0]);
    int *a=NULL;


    a = heapSort(b, len);
    for ( int i=1; i<len+1 ; i++ ){
        printf("%d \n", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
