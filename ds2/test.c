#include <stdio.h>
#include <stdlib.h>

#include  "anumber.h"


void
main() {
    ANumber *n1 = ANumber_newInt(123);

    printf("ANumber_intValue: %d \n", ANumber_intValue(n1));
    ANumberValueType t = ANumber_type(n1);
    printf("ANumber_type: %d \n", t);
    char *str = ANumber_typeToString((int) t);
    printf("type str--: %s \n", str);
}
