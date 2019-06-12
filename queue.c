#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//linked list Q
struct q_elem {
    void *data;
    struct q_elem *next;
};

struct my_q {
    struct q_elem *front;
    struct q_elem *rear;
};

struct my_q* initQ() {
    struct my_q *q = malloc(sizeof( struct my_q) );
    q->front = NULL;
    q->rear = NULL;

    return q;
}

void destroyQ(struct my_q *q){
    struct q_elem *e;

    e = q->front;
    while (e) {
        struct q_elem *f = e;
        e = e->next;
        free(f);
    }

    free(q);
}

bool isQempty(struct my_q *q) {
    return (q->front == NULL);
}

void addQ(struct my_q *q, void *data) {
    if (isQempty(q)) { //Q empty
        q->front = q->rear = malloc(sizeof(struct q_elem));
        q->front->next = NULL;
    } else { 
        q->rear->next = malloc(sizeof(struct q_elem));
        q->rear = q->rear->next;

        if( q->front->next == NULL )
            q->front->next = q->rear;
    }

    q->rear->data = data;
}
void * popQ(struct my_q *q) {
    void *ret=NULL;

    if(! isQempty(q)) {
        struct q_elem *f = q->front;
        ret = q->front->data;

        if(q->front == q->rear)
            q->front = q->rear = NULL;

        else
            q->front = q->front->next;

        free(f);
    }

    return ret;
}


int main() {
    struct my_q *q = initQ();
    int a[5]={0,1,2,3,4};
    void *b;

    for( int i=0; i<5; i++)
        addQ(q, &a[i]);

    for( int i=0; i<6; i++) {
        b = popQ(q);
        if (b)
            printf("%d\n", *(int *)b);
    }

    destroyQ(q);
    return 0;
}
