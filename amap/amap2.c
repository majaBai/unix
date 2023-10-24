
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap2.h"
#include "amap1.h"

AMap *
AMap_new(void){
    AMap *m = malloc(sizeof(AMap));
    m->data = AArray_new();

    return m;
}
void
AMap_set(AMap *m, AString *key, AString *value) {

    if(AArray_length(m->data) == 0) {
         AArray *kv = AArray_new();
         AArray_add(kv, key);
         AArray_add(kv, value);

         AArray_add(m->data, kv);
    } else {
        bool repeat = false;
        for(int i = 0; i < AArray_length(m->data); i++) {
            AArray *kv = m->data[i];

            AString *k = AArray_get(kv, 0);
            if(AString_equals(k, key)) {
                
            }

        }
    }
}