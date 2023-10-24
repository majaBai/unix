#include <stdlib.h>
#include <assert.h>

#include "astring.h"

unsigned int
_hash(AString *key) {
    unsigned int r = 0;
    for(int i = 0; i < key->length; i++) {
        r += key->data[i];
    }
    return r;
}