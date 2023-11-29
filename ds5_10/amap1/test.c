#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "amap1.h"


// cc amap1.c ../lib/*.c test.c -Ilib && ./a.out
void
main() {
    {
        AString *s = AString_new("axe5");
        unsigned int h = _hash(s);
        printf("h: %u \n", h);
    }
    
}