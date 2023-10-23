#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "alist.h"
#include "anumber.h"

void
main() {
    // ANumber *n1 = ANumber_newInt(123);
    // printf("ANumber_intValue: %d \n", ANumber_intValue(n1));
    // assert(ANumber_intValue(n1) == 123);

    AList *l = AList_new();
    assert(AList_length(l) == 0);
    // add
    {
        printf("add \n");
        ANumber *n = ANumber_newInt(12345);
        AList_add(l, n);
        {
            ANumber *n = ANumber_newInt(23456);
            AList_add(l, n);
        }
        {
            ANumber *n = ANumber_newInt(34567);
            AList_add(l, n);
        }
        {
            ANumber *n = ANumber_newInt(45678);
            AList_add(l, n);
        }
        printf("length add: %u \n", l->size);
    }

    {
        // get
        ANumber *n = AList_get(l, 0);
        printf("get:  %d \n", ANumber_intValue(n));
        assert(ANumber_intValue(n) == 12345);
    }
    {
        //pop
        ANumber *n = AList_pop(l);
        printf("pop: %d \n", ANumber_intValue(n));
        assert(ANumber_intValue(n) == 45678);
        printf("length pop: %u \n", l->size);
    }

    {
        // remove
        printf("remove \n");
        AList_removeAtIndex(l, 0);
        ANumber *n = AList_get(l, 0);
        assert(ANumber_intValue(n) == 23456);
        printf("length after remove: %u \n", l->size);
    }

    {
        // set insert
        AList *a = AList_new();
        {
            ANumber *n = ANumber_newInt(1);
            AList_add(a, n);
        }
        {
            ANumber *n = ANumber_newInt(22);
            AList_add(a, n);
        }
        {
            ANumber *n = ANumber_newInt(333);
            AList_add(a, n);
        }
        // [1, 22, 333]
        printf("lengt after add: %u \n", a->size);

        {
            //set
            printf("set \n");
            {
                ANumber *n = AList_get(a, 1);
                assert(ANumber_intValue(n) == 22);
            }
            ANumber *n = ANumber_newInt(4444);
            AList_set(a, 1, n);
            {
                ANumber *n = AList_get(a, 1);
                assert(ANumber_intValue(n) == 4444);
            }
            // [1, 4444, 333]
        }

        {
            // insert
            printf("insert \n");
            {
                ANumber *n = AList_get(a, 0);
                assert(ANumber_intValue(n) == 1);
            }
            ANumber *n = ANumber_newInt(55);
            AList_insertAtIndex(a, 0, n);
            {
                ANumber *n = AList_get(a, 0);
                assert(ANumber_intValue(n) == 55);
                printf("len after insert: %u \n", a->size);
            }
            // [55, 1, 4444, 333]

        }



    }


}