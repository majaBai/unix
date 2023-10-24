#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap1.h"

void
main() {
    {
        // amap1
        AString *s = AString_new("axe5");
        unsigned int h = _hash(s);
        printf("h: %u \n", h);
    }
    
}