#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap6.h"

// cc amap6.c ../lib/*.c test.c -Ilib && ./a.out
void
main() {
        AMap * am = AMap_new();
        AString *k = AString_new("maja");
        AString *v = AString_new("178");
        AMap_set(am, k, v);

        AString *r = AMap_get(am, k);
        printf("get maja: %s\n", r->data);

         printf("map count: %u ; cap: %u\n", am->count, am->capacity);

    // add(set)
        {
            AString *k = AString_new("shelly");
            AString *v = AString_new("170");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("bob");
            AString *v = AString_new("170");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("scan");
            AString *v = AString_new("170");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("julice");
            AString *v = AString_new("140");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("jerry");
            AString *v = AString_new("188");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("merley");
            AString *v = AString_new("166");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("xiaomei");
            AString *v = AString_new("177");
            AMap_set(am, k, v);
        }
        printf("map count: %u ; cap: %u\n", am->count, am->capacity);
        {
           // has
           AString *k = AString_new("bob");
           bool r = AMap_has(am, k);
           printf("has [bob] ture: %d\n", r);
        }
         {
           // has
           AString *k = AString_new("shelly");
           bool r = AMap_has(am, k);
           printf("has [shelly] true : %d\n", r);
        }
    
}