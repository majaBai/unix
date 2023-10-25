#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap2.h"

// cc amap2.c ../lib/*.c test.c -Ilib && ./a.out
void
main() {
    {
        AMap * am = AMap_new();
        AString *k = AString_new("maja");
        AString *v = AString_new("178");
        AMap_set(am, k, v);

        AString *r = AMap_get(am, k);
        printf("get maja: %s\n", r->data);

        {
            AString *k = AString_new("scan");
            AString *v = AString_new("180");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("bigDan");
            AString *v = AString_new("160");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("shelly");
            AString *v = AString_new("170");
            AMap_set(am, k, v);
        }
        {
            AString *k = AString_new("maja");
            AString *v = AString_new("190");
            AMap_set(am, k, v);
        }
        {
            // get
            AString *r = AMap_get(am, k);
           printf("get maja after set : %s\n", r->data);
        }
        {
           // has
           AString *k = AString_new("shelly");
           bool r = AMap_has(am, k);
           printf("has [shelly]: true %d\n", r);
        }
         {
           // has
           AString *k = AString_new("bob");
           bool r = AMap_has(am, k);
           printf("has [bob]: false %d\n", r);
        }
    }
    
}