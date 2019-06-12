#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    H_STRING,
    H_UL,
} hash_type_t;

struct hash_elem {
    union {
        char * str;
        unsigned long ul;
    } key;
    void **val;
    int val_cnt;
    struct hash_elem *next;
    hash_type_t type;
};

struct hashtable {
    struct hash_elem **bucket;
    int sz;
    hash_type_t type;
};

struct hashtable * initHT(int sz, hash_type_t type) {
    struct hashtable *ht = malloc(sizeof(struct hashtable));
    ht->bucket = malloc(sizeof(struct hash_elem *) * sz);

    for( int i=0 ; i < sz; i++) {
        ht->bucket[i] = NULL;
    }

    ht->sz = sz;
    ht->type = type;

    return ht;
}

void destroyHT(struct hashtable *ht, int sz) {
    for(int i=0; i<sz ;i++) {
        struct hash_elem *e, *next_e;
        e = ht->bucket[i];
        while (e) {
            if( ht->type == H_STRING ){
                if (e->key.str)
                    free(e->key.str);
            }
            if (e->val) {
                free(e->val);
            }

            next_e = e->next;
            free(e);
            e = next_e;
        }
    }
    free(ht->bucket);
    free(ht);
}

unsigned long hash_str(char *key) {
    unsigned long seed = 13131, hash=0;
    char c;
    
    while((c = *key++)) {
        hash = c + hash * seed;
    }

    return hash;
}

unsigned long hash_ul(unsigned long addr) {
    return addr;
}

unsigned long hash(void *key, hash_type_t type, int htSZ) {
    unsigned long hashv;
    if( type == H_STRING ) {
        hashv = hash_str((char *)key) % htSZ;
            
    } else { //H_UL
        hashv = hash_ul(*(unsigned long *)key) % htSZ;
    }
    return hashv;
}

struct hash_elem * searchHT(struct hashtable *ht, void *key) {
    unsigned long hashv;
    struct hash_elem *e = NULL;

    hashv = hash(key, ht->type, ht->sz);

    if ( ht->type == H_STRING) {
        e = ht->bucket[hashv];
        while (e) {
            if(strcmp(e->key.str, key) == 0)
                break;
            e = e->next;
        }
    } else { //H_UL
        e = ht->bucket[hashv];
        while (e) {
            if(e->key.ul == *(unsigned long *)key)
                break;
            e = e->next;
        }
    }

    return e;
}

void insertHT(struct hashtable *ht, void *key, void *val) {
    struct hash_elem **ppe;
    struct hash_elem *pe;
    unsigned long hashv;

    hashv = hash(key, ht->type, ht->sz);
    ppe = &ht->bucket[hashv];
    //allocate first
    if (*ppe == NULL ) {
        *ppe = malloc( sizeof(struct hash_elem) );
        *ppe = ht->bucket[hashv];
        if( ht->type == H_STRING ) {
            (*ppe)->key.str = NULL;
            (*ppe)->val = NULL;
        } else { //H_UL
            (*ppe)->key.ul = 0;
            (*ppe)->val = NULL;
        }
        (*ppe)->val_cnt = 0;
        (*ppe)->next = NULL;
    }

    pe = *ppe;
    struct hash_elem *prev_e;
    while (pe) {
        prev_e = pe;
        if ( ( ht->type == H_STRING && pe->key.str != NULL && strcmp(pe->key.str, key) == 0) || \
            ( ht->type == H_UL && pe->key.ul == *(unsigned long *)key)) {//same key, extend value
                pe->val_cnt +=1;
                pe->val = realloc(pe->val, sizeof(void *) * pe->val_cnt);
                pe->val[pe->val_cnt - 1] = val;
                pe->type = ht->type;
                return;
        }
        pe = pe->next;
    }

    if ( prev_e )
        pe = prev_e;

    if (pe->val_cnt > 0) { //collision case 
        pe->next = malloc(sizeof(struct hash_elem));
        pe = prev_e->next;
        pe->val = NULL;
        pe->val_cnt = 0;
    }

    if ( ht->type == H_STRING )
        pe->key.str = strdup(key);
    else //H_UL
        pe->key.ul = *(unsigned long *)key;
        
    pe->val_cnt += 1;
    pe->val = realloc(pe->val, sizeof(void *) * pe->val_cnt);
    pe->val[pe->val_cnt - 1] = val;
    pe->next = NULL;
    pe->type = ht->type;
}

int main() {
    int str_ht_sz = 5;
    int ul_ht_sz = 5;
    struct hash_elem *str_elem, *ul_elem;

    struct hashtable *str_ht = initHT(str_ht_sz, H_STRING);
    struct hashtable *ul_ht = initHT(ul_ht_sz, H_UL);

    insertHT(str_ht, (void *)"abc", "123");
    insertHT(str_ht, (void *)"abc", "333");
    insertHT(str_ht, (void *)"hhh", "1672");
    insertHT(str_ht, (void *)"osj", "5334");
    insertHT(str_ht, (void *)"ald", "745");
    insertHT(str_ht, (void *)"darren", "8493");
    str_elem = searchHT(str_ht, (void *)"abc");
    if ( str_elem ) {
        for(int i=0; i<str_elem->val_cnt; i++) {
            printf("str: %s\n", (char *)str_elem->val[i]);
        }
    }
    unsigned long k =3;
    unsigned long v = 323;
    insertHT(ul_ht, (void *)&k, &v);
    k =3;
    v = 987;
    insertHT(ul_ht, (void *)&k, &v);
    k =6;
    v = 666;
    insertHT(ul_ht, (void *)&k, &v);
    k =8;
    v = 888;
    insertHT(ul_ht, (void *)&k, &v);
    k = 10;
    v = 10101;
    insertHT(ul_ht, (void *)&k, &v);
    k = 11;
    v = 11111;
    insertHT(ul_ht, (void *)&k, &v);
    k = 3;
    ul_elem = searchHT(ul_ht, &k);
    if (ul_elem) {
        for(int i=0; i<ul_elem->val_cnt; i++) {
            printf("ul: %lu\n", *(unsigned long *)ul_elem->val[i]);
        }
    }

    destroyHT(str_ht, str_ht_sz);
    destroyHT(ul_ht, ul_ht_sz);
    return 0;
}
