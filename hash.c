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
    union {
        char **pstr;
        unsigned long *ul;
    } val;
    int val_cnt;
    struct hash_elem *next;
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
                if (e->val.pstr) {
                    for (int j=0; j<e->val_cnt; j++)
                        if (e->val.pstr[j])
                            free(e->val.pstr[j]);
                    free(e->val.pstr);
                }
            } else {// H_UL
                if (e->val.ul)
                    free(e->val.ul);
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

struct hash_elem * searchHT(struct hashtable *ht, void *key, int htSZ) {
    unsigned long hashv;
    struct hash_elem *e = NULL;

    hashv = hash(key, ht->type, htSZ);

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

void insertHT(struct hashtable *ht, void *key, void *val, int htSZ) {
    struct hash_elem **ppe;
    struct hash_elem *pe;
    unsigned long hashv;

    hashv = hash(key, ht->type, htSZ);
    ppe = &ht->bucket[hashv];
    //allocate first
    if (*ppe == NULL ) {
        *ppe = malloc( sizeof(struct hash_elem) );
        *ppe = ht->bucket[hashv];
        if( ht->type == H_STRING ) {
            (*ppe)->key.str = NULL;
            (*ppe)->val.pstr = NULL;
        } else { //H_UL
            (*ppe)->key.ul = 0;
            (*ppe)->val.ul = NULL;
        }
        (*ppe)->val_cnt = 0;
        (*ppe)->next = NULL;
    }

    pe = *ppe;
    if ( ht->type == H_STRING ) {
        struct hash_elem *prev_e;
        while (pe) {
            prev_e = pe;
            if( pe->key.str != NULL && strcmp(pe->key.str, key) == 0) { //same key, extend value
                pe->val_cnt +=1;

                pe->val.pstr = realloc(pe->val.pstr, sizeof(char *) * pe->val_cnt);
                pe->val.pstr[pe->val_cnt - 1] = strdup(val);
                return;
            }
            pe = pe->next;
        }

        if ( prev_e )
            pe = prev_e;

        if (pe->val_cnt > 0) { //collision case 
            pe->next = malloc(sizeof(struct hash_elem));
            pe = prev_e->next;
            pe->val.pstr = NULL;
            pe->val_cnt = 0;
        }

        pe->key.str = strdup(key);
        pe->val_cnt += 1;
        pe->val.pstr = realloc(pe->val.pstr, sizeof(char *) * pe->val_cnt);
        pe->val.pstr[pe->val_cnt - 1] = strdup(val);
        pe->next = NULL;

    } else { //H_UL
        struct hash_elem *prev_e;
        while (pe) {
            prev_e = pe;
            if( pe->key.ul == *(unsigned long *)key) { //same key, extend value
                pe->val_cnt +=1;

                pe->val.ul = realloc(pe->val.ul, sizeof(unsigned long) * pe->val_cnt);
                pe->val.ul[pe->val_cnt - 1] = *(unsigned long *)val;
                return;
            }
            pe = pe->next;
        }

        if ( prev_e )
            pe = prev_e;

        if (pe->val_cnt > 0) { //collision case 
            pe->next = malloc(sizeof(struct hash_elem));
            pe = prev_e->next;
            pe->val.ul = NULL;
            pe->val_cnt = 0;
        }

        pe->key.ul = *(unsigned long *)key;
        pe->val_cnt += 1;
        pe->val.ul = realloc(pe->val.ul, sizeof(unsigned long) * pe->val_cnt);
        pe->val.ul[pe->val_cnt - 1] = *(unsigned long *) val;
        pe->next = NULL;
    }
    
}

int main() {
    int str_ht_sz = 5;
    int ul_ht_sz = 5;
    struct hash_elem *str_elem, *ul_elem;

    struct hashtable *str_ht = initHT(str_ht_sz, H_STRING);
    struct hashtable *ul_ht = initHT(ul_ht_sz, H_UL);

    insertHT(str_ht, (void *)"abc", "123", str_ht_sz);
    insertHT(str_ht, (void *)"abc", "333", str_ht_sz);
    insertHT(str_ht, (void *)"hhh", "1672", str_ht_sz);
    insertHT(str_ht, (void *)"osj", "5334", str_ht_sz);
    insertHT(str_ht, (void *)"ald", "745", str_ht_sz);
    insertHT(str_ht, (void *)"darren", "8493", str_ht_sz);
    str_elem = searchHT(str_ht, (void *)"darren", str_ht_sz);
    if ( str_elem ) {
        for(int i=0; i<str_elem->val_cnt; i++) {
            printf("%s\n", str_elem->val.pstr[i]);
        }
    }
    unsigned long k =3;
    unsigned long v = 323;
    insertHT(ul_ht, (void *)&k, &v, ul_ht_sz);
    k =3;
    v = 987;
    insertHT(ul_ht, (void *)&k, &v, ul_ht_sz);
    k =6;
    v = 666;
    insertHT(ul_ht, (void *)&k, &v, ul_ht_sz);
    k =8;
    v = 888;
    insertHT(ul_ht, (void *)&k, &v, ul_ht_sz);
    k = 10;
    v = 10101;
    insertHT(ul_ht, (void *)&k, &v, ul_ht_sz);
    k = 11;
    v = 11111;
    insertHT(ul_ht, (void *)&k, &v, ul_ht_sz);
    k = 3;
    ul_elem = searchHT(ul_ht, &k, ul_ht_sz);
    if (ul_elem) {
        for(int i=0; i<ul_elem->val_cnt; i++) {
            printf("%lu\n", ul_elem->val.ul[i]);
        }
    }

    destroyHT(str_ht, str_ht_sz);
    destroyHT(ul_ht, ul_ht_sz);
    return 0;
}
