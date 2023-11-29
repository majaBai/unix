
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap2.h"

AMap *
AMap_new(void){
    AMap *m = malloc(sizeof(AMap));
    m->data = AArray_new();

    return m;
}
void
AMap_set(AMap *m, AString *key, AString *value) {
    for(int i = 0; i < AArray_length(m->data); i++) {
        AArray *kv = AArray_get(m->data, i);

        AString *k = AArray_get(kv, 0);
        if(AString_equals(k, key)) {
            AArray_set(kv, 1, value);
            return;
        }
    }
    AArray *kv = AArray_new();
    AArray_add(kv, key);
    AArray_add(kv, value);

    AArray_add(m->data, kv);
}

AString *
AMap_get(AMap *m, AString *key){
    for(int i = 0; i < AArray_length(m->data); i++) {
        AArray *kv = AArray_get(m->data, i);

        AString *k = AArray_get(kv, 0);
        if(AString_equals(k, key)) {
            return AArray_get(kv, 1);
        }
    }
    return NULL;
}

bool
AMap_has(AMap *m, AString *key){
    for(int i = 0; i < AArray_length(m->data); i++) {
        AArray *kv = AArray_get(m->data, i);

        AString *k = AArray_get(kv, 0);
        if(AString_equals(k, key)) {
            return true;
        }
    }
    return false;
}