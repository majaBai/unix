#include <stdio.h>
#include <stdlib.h>
#include "aarray.h"

void
main() {
    // 创建一个新的 AArray
    AArray *array = AArray_new();


    // 添加
    int num1 = 42;
    AArray_add(array, (void *)&num1);

    int num2 = 17;
    AArray_add(array, (void *)&num2);

    int num3 = 99;
    AArray_add(array, (void *)&num3);

    int num4 = 199;
    int num5 = 399;

    int num6 = 689;
    AArray_add(array, (void *)&num6);

    int num7 = 854;
    AArray_add(array, (void *)&num7);

    int num8 = 532;
    AArray_add(array, (void *)&num8);

    int num9 = 1000;


    printf("after add---- %ld\n", AArray_length(array));
    for (int i = 0; i < AArray_length(array); i++) {
        int *element = (int *)AArray_get(array, i);
        printf(" %d: %d\n", i,*element);
    }

    
    AArray_set(array, 2, (void *)&num4);
    AArray_set(array, 1, (void *)&num5);

    printf("after set---- %ld\n", AArray_length(array));
    for (int i = 0; i < AArray_length(array); i++) {
        int *element = (int *)AArray_get(array, i);
        printf(" %d: %d\n", i,*element);
    }

    AArray_removeAtIndex(array, 0);
    printf("after remove---- %ld\n", AArray_length(array));
    for (int i = 0; i < AArray_length(array); i++) {
        int *element = (int *)AArray_get(array, i);
        printf(" %d: %d\n", i,*element);
    }

    int * pop = (int *)AArray_pop(array);
    printf("poped: %d\n", *pop);

    printf("after pop---- %ld\n", AArray_length(array));
    for (int i = 0; i < AArray_length(array); i++) {
        int *element = (int *)AArray_get(array, i);
        printf(" %d: %d\n", i,*element);
    }

    AArray_insertAtIndex(array, 0, (void *)&num9);
    printf("after insert---- %ld \n", AArray_length(array));
    for (int i = 0; i < AArray_length(array); i++) {
        int *element = (int *)AArray_get(array, i);
        printf(" %d: %d\n", i,*element);
    }

    // 释放内存
    // AArray_destroy(array);
}