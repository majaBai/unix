
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap3.h"

AMap *
AMap_new(void){
    AMap *m = malloc(sizeof(AMap));
    for(int i = 0; i < 1; i++) {
        m->data[i] = AArray_new();
    }
    return m;
}
void
AMap_set(AMap *m, AString *key, AString *value) {
    // printf("set--- %ld \n", AArray_length(m->data[0]));
    for(int i = 0; i < AArray_length(m->data[0]); i++) {
        AArray *kv = AArray_get(m->data[0], i);
        AString *k = AArray_get(kv, 0);
        if(AString_equals(k, key)) {
            AArray_set(kv, 1, value);
            return;
        }
    }
    AArray *kv = AArray_new();
    AArray_add(kv, key);
    AArray_add(kv, value);

    AArray_add(m->data[0], kv);
}

AString *
AMap_get(AMap *m, AString *key){
    for(int i = 0; i < AArray_length(m->data[0]); i++) {
        AArray *kv = AArray_get(m->data[0], i);

        AString *k = AArray_get(kv, 0);
        if(AString_equals(k, key)) {
            return AArray_get(kv, 1);
        }
    }
    return NULL;
}

bool
AMap_has(AMap *m, AString *key){
    for(int i = 0; i < AArray_length(m->data[0]); i++) {
        AArray *kv = AArray_get(m->data[0], i);

        AString *k = AArray_get(kv, 0);
        if(AString_equals(k, key)) {
            return true;
        }
    }
    return false;
}