#include <stdio.h>
#include <stdlib.h>

struct my_stack {
    void **data;
    int sz;
};

struct my_stack * initStack() {
    struct my_stack *s = malloc(sizeof(struct my_stack));
    s->data = NULL;
    s->sz = 0;
    return s;
}

void destroyStack(struct my_stack *s) {
    if (s == NULL) return;
    free(s->data);        
    free(s);
}

void pushStack(struct my_stack *s, void *data) {
    if (s==NULL) return;
   
    s->sz +=1;
    s->data = realloc(s->data, sizeof(void *) * s->sz);
    s->data[s->sz -1] = data;
}

void *popStack(struct my_stack *s) {
    if (s==NULL) return NULL;
    
    void *ret=NULL;
    if( s->sz > 0) {
        s->sz -=1;
        ret = s->data[s->sz];
        s->data = realloc(s->data, sizeof(void *) *s->sz);
    }
    return ret;
}

int main() {
    struct my_stack *s = initStack();
    int a[5] = {1,2,3,4,5};
    void *b;
    
    for (int i=0; i<3;i++) {
        pushStack(s, &a[i]);
    }
    for (int i=0; i<2 ; i++) {
        b = popStack(s);
        if( b )
            printf("%d\n", *(int *)b);
    }
    for (int i=3; i<5;i++) {
        pushStack(s, &a[i]);
    }
    for (int i=0; i<5 ; i++) {
        b = popStack(s);
        if( b )
            printf("%d\n", *(int *)b);
    }
    destroyStack(s);
}
