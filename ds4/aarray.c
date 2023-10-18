
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aarray.h"


// 创建新的 AArray
AArray *AArray_new(void) {
    AArray *array = malloc(sizeof(AArray));
    array->len = 0;
    array->base = NULL;
    return array;
}

long
AArray_length(AArray *array) {
    
    return array->len;
}

// 取出元素
void *
AArray_get(AArray *array, long index) {
    if (index < 0 || index >= array->len) {
        perror("下标越界");
        return NULL; // 下标越界
    }
    return array->base[index];
}

// 添加元素在末尾
void 
AArray_add(AArray *array, void *element) {
    // 增加len
    array->len +=1;
    // 扩容并拷贝旧数据到新内存中
    void **newBase = (void**)malloc(sizeof(void*) * array->len);
    memcpy(newBase, array->base, sizeof(void*) * (array->len - 1));

    // 在末尾添加
     newBase[array->len - 1] = element;
    free(array->base); // Free the old base
    array->base = newBase; 
}

void
AArray_set(AArray *array, long index, void *element) {
    array->base[index] = element;
}

void *
AArray_pop(AArray *array) {
    void * res = array->base[array->len - 1];
    array->len -= 1;

    void **newBase = (void**)malloc(sizeof(void*) * array->len);
    memcpy(newBase, array->base, sizeof(void*) * array->len);

    free(array->base);
    array->base = newBase;

    return res;
}

void
AArray_destroy(AArray *array) {
    free(array->base);
    free(array);
    array = NULL;
}

void
AArray_removeAtIndex(AArray *array, long index) {
    void **newBase = (void**)malloc(sizeof(void*) * array->len - 1);
    int found = 0;
    for(long i = 0; i < array->len; i++)  {
        if(i == index) {
            found = 1;
        } else {
            if(found == 1) {
              newBase[i -1] = array->base[i];
            } else {
              newBase[i] = array->base[i];
            }
        }
    }
    free(array->base);
    array->base = newBase;
    array->len -=1;
}

void
AArray_insertAtIndex(AArray *array, long index, void *element) {
    void **newBase = (void**)malloc(sizeof(void*) * array->len + 1);
    int found = 0;
    for(long i = 0; i < array->len + 1; i++)  {
        if(i == index) {
            found = 1;
            newBase[i] = element;
        } else {
            if(found == 1) {
              newBase[i] = array->base[i - 1];
            } else {
              newBase[i] = array->base[i];
            }
        }
    }
    free(array->base);
    array->base = newBase;
    array->len +=1;
}