
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aarray.h"


// 创建新的 AArray
AArray *AArray_new(void) {
    AArray *array = malloc(sizeof(AArray));

    unsigned int cap = 10;
    array->capacity = cap;
    array->data = malloc(cap * sizeof(void *));
    array->length = 0;

    return array;
}

long
AArray_length(AArray *array) {
    return array->length;
}

// 取出元素
void *
AArray_get(AArray *array, long index) {
    assert(index < array->length);
    if(array->length > 100000){
         printf("AArray_get= %lu\n", AArray_length(array));
    }
    return array->data[index];
}

// 添加元素在末尾
void 
AArray_add(AArray *array, void *element) {

    // 先检查是否需要扩容
    if(array->length == array->capacity) {
        // printf("@@@AArray 数组扩容：%lu \n", array->capacity);
        unsigned int cap = array->capacity * 2;
        void **new_data = (void **)malloc(cap * sizeof(void *));
        memcpy(new_data, array->data, sizeof(void*) * array->length);

        free(array->data);
        array->data = new_data;
        array->capacity = cap;
    }
    array->data[array->length] = element;
    array->length +=1;
    // printf("@@@after add：%lu \n", array->length);
}

void
AArray_set(AArray *array, long index, void *element) {
    array->data[index] = element;
}

void *
AArray_pop(AArray *array) {
    assert(array->length > 0);

    void * element = array->data[array->length - 1];
    array->length -= 1;
    return element;
}

void
AArray_destroy(AArray *array) {
    free(array->data);
    free(array);
    array = NULL;
}

void
AArray_removeAtIndex(AArray *array, long index) {
    assert(index < array->length);

    long lastIdx = array->length - 1;
    long numOfMoves = lastIdx - index;
    for(int i = 0; i < numOfMoves; i++) {
        int idx1 = index + i;
        int idx2 = index + i + 1;
        array->data[idx1] = array->data[idx2];
    }
    array->length -=1;
}

void
AArray_insertAtIndex(AArray *array, long index, void *element) {
    assert(index <= array->length);
   if(array->length == array->capacity) {
        unsigned int cap = array->capacity * 2;
        void **new_data = (void **)malloc(cap * sizeof(void *));
        memcpy(new_data, array->data, sizeof(void*) * array->length);

        free(array->data);
        array->data = new_data;
        array->capacity = cap;
    }
    array->length +=1;
    for(int i = array->length -1; i > index; i--) {
        array->data[i] = array->data[i - 1];
    }
    array->data[index] = element;
}