#include <stdio.h>
#include <stdlib.h>
#include "alist.h"

void
main() {
    AList *l = AList_new();


    // add
    // 下面写法等价
    //  void * p1 = NULL;
    // int val1 = 123;
    // p1 = &val1;
    // ANode *n1 = ANode_new(p1, 0);
    int val1 = 123;
    ANode *n1 = ANode_new(&val1, 0);
    AList_add(l, n1);

    void * p2 = "axe5-axe5-axe5-axe5-axe5-axe5";
    ANode *n2 = ANode_new(p2, 1);
    AList_add(l, n2);

    int val3 = 567;
    ANode *n3 = ANode_new(&val3, 0);
    AList_add(l, n3);

    AList_print(l);

    // set
    void * val4 = "set_new_axe5";
    int val5 = 888;
    AList_set(l, 0, &val5, 0);
    AList_set(l, 1, val4, 1);
    AList_set(l, 2, val4, 1);
    AList_print(l);

    // pop
    // AList_pop(l);
    // AList_print(l);

    // remove at idx
    AList_removeAtIndex(l, 0);
    AList_print(l);

    free(l);

}