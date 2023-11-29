
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap5.h"

unsigned int
_hash(AString *key) {
    unsigned int r = 0;
    for(int i = 0; i < key->length; i++) {
        r += key->data[i];
    }
    return r;
}

AMap *
AMap_new(void){
    AMap *m = malloc(sizeof(AMap));
    m->capacity = 101;
    m->data = malloc(sizeof(AArray *) * m->capacity);
    for(int i =0; i < m->capacity; i++) {
        m->data[i] = AArray_new();
    }
    return m;
}
void
AMap_set(AMap *m, AString *key, AString *value) {
     unsigned int idx = _hash(key) % (m->capacity);
     AArray *data = m->data[idx]; 
     // 遍历是否key存在
     for(int i = 0; i < AArray_length(data); i++) {
        AArray *kv = AArray_get(data, i);
        AString *k = AArray_get(kv, 0);
        if(AString_equals(key, k)) {
            AArray_set(kv, 1, value);
            return;
        }
     }
     AArray *kv = AArray_new();
     AArray_add(kv, key);
     AArray_add(kv, value);
     AArray_add(data, kv);
}

AString *
AMap_get(AMap *m, AString *key){
    unsigned int idx = _hash(key) % (m->capacity);
    AArray *data = m->data[idx];
    for(int i = 0; i < AArray_length(data); i++) {
        AArray *kv = AArray_get(data, i);
        AString *k = AArray_get(kv, 0);
        if(AString_equals(key, k)) {
            return AArray_get(kv, 1);
        }
     }
     return NULL;
}

bool
AMap_has(AMap *m, AString *key){
   unsigned int idx = _hash(key) % (m->capacity);
    AArray *data = m->data[idx];
    for(int i = 0; i < AArray_length(data); i++) {
        AArray *kv = AArray_get(data, i);
        AString *k = AArray_get(kv, 0);
        if(AString_equals(key, k)) {
            return true;
        }
     }
     return false;
}