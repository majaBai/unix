#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "aarray.h"
#include "anumber.h"

void
main() {
    // 创建一个新的 AArray
    AArray *arr = AArray_new();
    assert(arr->length == 0);

    // add
    {
        printf("add \n");
        ANumber *n = ANumber_newInt(12345);
        AArray_add(arr, n);
        {
            ANumber *n = ANumber_newInt(23456);
            AArray_add(arr, n);
        }
        {
            ANumber *n = ANumber_newInt(34567);
            AArray_add(arr, n);
        }
        {
            ANumber *n = ANumber_newInt(45678);
            AArray_add(arr, n);
        }
        printf("len add: %u \n", arr->length);
    }

    {
        // get
        ANumber *n = AArray_get(arr, 0);
        printf("get [0]: %d \n", ANumber_intValue(n));
        {
            ANumber *n = AArray_get(arr, 1);
            printf("get [1]: %d \n", ANumber_intValue(n));
        }
        {
            ANumber *n = AArray_get(arr, 3);
            printf("get [3]: %d \n", ANumber_intValue(n));
        }

    }

    {
        // set
        ANumber *n = ANumber_newInt(111);
        AArray_set(arr, 0, n);
        printf("set [0]=111: %d \n", ANumber_intValue(AArray_get(arr, 0)));
        {
            ANumber *n = ANumber_newInt(222);
            AArray_set(arr, 1, n);
            printf("set [1]=222: %d \n", ANumber_intValue(AArray_get(arr, 1)));
        }
        {
            ANumber *n = ANumber_newInt(444);
            AArray_set(arr, 3, n);
            printf("set [3]=444: %d \n", ANumber_intValue(AArray_get(arr, 3)));
        }
    }
    {
        //pop
        ANumber * n = AArray_pop(arr);
        printf("len pop: %u, ele: %d ; after pop: \n", arr->length, ANumber_intValue(n));
         printf("[0]: %d \n", ANumber_intValue(AArray_get(arr, 0)));
        printf("[1]: %d \n", ANumber_intValue(AArray_get(arr, 1)));
        printf("[2]: %d \n", ANumber_intValue(AArray_get(arr, 2)));
    }

    {
        // insert
        ANumber *n = ANumber_newInt(555);
        AArray_insertAtIndex(arr, 3, n);
        {
            ANumber *n = ANumber_newInt(666);
            AArray_insertAtIndex(arr, 1, n);
        }
        {
            ANumber *n = ANumber_newInt(777);
            AArray_insertAtIndex(arr, 2, n);
        }
        printf("insert [0]: %d \n", ANumber_intValue(AArray_get(arr, 0)));
        printf("insert [1]: %d \n", ANumber_intValue(AArray_get(arr, 1)));
        printf("insert [2]: %d \n", ANumber_intValue(AArray_get(arr, 2)));
        printf("insert [3]: %d \n", ANumber_intValue(AArray_get(arr, 3)));
        printf("insert [4]: %d \n", ANumber_intValue(AArray_get(arr, 4)));
        printf("insert [5]: %d \n", ANumber_intValue(AArray_get(arr, 5)));
        printf("len insert: %u \n", arr->length);

    }
    {
        // remove
        AArray_removeAtIndex(arr, 5);
        printf("remove [4]: %d \n", ANumber_intValue(AArray_get(arr, 4)));

        {
            AArray_removeAtIndex(arr, 0);
            printf("remove [0]: %d \n", ANumber_intValue(AArray_get(arr, 0)));
        }
            printf("len remove: %u \n", arr->length);
    }
    
}