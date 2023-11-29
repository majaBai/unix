#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap3.h"

// cc amap3.c ../lib/*.c test.c -Ilib && ./a.out
void
main() {
        AMap * am = AMap_new();
        AString *k = AString_new("maja");
        AString *v = AString_new("178");
        AMap_set(am, k, v);

        AString *r = AMap_get(am, k);
        printf("get maja: %s\n", r->data);

    //     // add(set)
        {
            AString *k = AString_new("shelly");
            AString *v = AString_new("170");
            AMap_set(am, k, v);
        }
    //     {
    //         AString *k = AString_new("maja");
    //         AString *v = AString_new("190");
    //         AMap_set(am, k, v);
    //     }
    //     {
    //         // set
    //         AString *r = AMap_get(am, k);
    //        printf("get maja after set : %s\n", r->data);
    //     }
        {
           // has
           AString *k = AString_new("maja");
           bool r = AMap_has(am, k);
           printf("has [maja] false: %d\n", r);
        }
         {
           // has
           AString *k = AString_new("shelly");
           bool r = AMap_has(am, k);
           printf("has [shelly] true : %d\n", r);
        }
    
}