
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "map.h"
/*
现在创建 map 的时候会让 data 里的每个元素都 AArray_new 一次
可以改为默认它是 NULL，使用中按需创建
*/

unsigned int
_hash(AString *key) {
   unsigned int r = 0;
    for(int i = 0; i < key->length; i++) {
        char c = AString_charAt(key, i);
        r += c;
        r *= ((i + 1) * 10);
    }
    return r;
}
AMap *
Amap_newWithCap(int cap) {
    AMap *m = malloc(sizeof(AMap));
    m->capacity = cap;
    m->count = 0;
    m->data = malloc(sizeof(AArray *) * m->capacity);
    for(int i =0; i < m->capacity; i++) {
        // m->data[i] = AArray_new();
        m->data[i] = NULL;
    }
    return m;
}
AMap *
AMap_new(void){
    return Amap_newWithCap(10);
}
void
_rehash(AMap *m){
    // printf("rehash--- \n");
   AMap *m1 = Amap_newWithCap(m->capacity * 2);
   for(int i = 0; i < m->capacity; i++) {
    if(m->data[i] == NULL) {
        continue;
    }
    AArray *data = m->data[i];
    for(int j = 0; j < AArray_length(data); j++) {        
        AArray * kv = AArray_get(data, j);        
        AString *k = AArray_get(kv, 0);
        AString *v = AArray_get(kv, 1);
        AMap_set(m1, k, v);
    }
   }
   
   free(m->data);
   m->data = m1->data;
   m->capacity = m1->capacity;
   free(m1);
}
void
AMap_set(AMap *m, AString *key, AString *value) {
    // printf("set1--- \n");
    if((float)(m->count) / (float)m->capacity > 0.8) {
        _rehash(m);
    }
     unsigned int idx = _hash(key) % (m->capacity);
     if(m->data[idx] == NULL) {
        m->data[idx] = AArray_new();
     }
    //  printf("set--- \n");
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

     m->count += 1;
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

void
AMap_destroy(AMap* m) {
    free(m->data);
    free(m);
}