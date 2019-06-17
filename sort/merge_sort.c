#include <stdio.h>
#include <stdlib.h>

void Msort(int *a, int start, int end ,int* buf){
    if(start >= end) return;

    int mid=start + (end-start)/2;
    Msort(a, start, mid, buf);
    Msort(a, mid+1, end, buf);

    for(int i=start ; i<=end; i++) {
        buf[i] = a[i];
    }

    int p=start, p1=start, p2=mid+1;
    while(p1 <= mid &&  p2 <= end) {
        if(buf[p2] < buf[p1]) {
            a[p++] = buf[p2++];
        } else {
            a[p++] = buf[p1++];
        }
    }

    while(p1 <= mid) {
        a[p++] = buf[p1++];
    }
    
}

void merge_sort(int *a, int len) {
    int *buf = malloc(sizeof(int) * len);

    Msort(a, 0, len-1, buf);

    free(buf);
}

int main() {
    int b[] = { 2,6, 5, 8, 32, 3, 9, 87, 8, 7, 1, 0 };
    int len = sizeof(b)/sizeof(b[0]);
    int *a=malloc(sizeof(int) * len);
    for( int i=0; i< len ; i++)
        a[i] = b[i];

    merge_sort(a, len);

    for ( int i=0; i<len ; i++ ){
        printf("%d \n", a[i]);
    }   
    printf("\n");

    free(a);
    return 0;
}
