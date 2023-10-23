
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "alist.h"

ANode *
ANode_new(void * val){
    ANode *n = malloc(sizeof(ANode));
    n->value = val;
    n->next = NULL;

    return n;
}

AList *
AList_new(void) {
    AList *list = malloc(sizeof(AList));
    list->size = 0;
    list->first = NULL;
    return list;
}
long
AList_length(AList *array) {
    return array->size;
}
// 添加元素在末尾
//  void * 万能指针，可以指向任何数据类型
void
AList_add(AList *array, void *element) {
    ANode *n = malloc(sizeof(ANode));
    (*n) = (ANode) {
        .value = element,
        .next = NULL,
    };

    // 第1次添加
    if(array->size == 0 ) {
        array->first = n;
    } else {
        ANode *cur = array->first;
        while(cur->next != NULL) {
          cur = cur->next;
        }
        cur->next = n;
    }
    array->size += 1;
}

void *
AList_get(AList *array, long index) {
    assert(index < array->size);

    ANode *res = array->first;
    for(int i = 0; i < index; i++) {
        res = res->next;
    }
    return res->value;
}

void
AList_insertAtIndex(AList *array, long index, void *element){
    assert(index < array->size);

    ANode *newNode = malloc(sizeof(ANode));
    (*newNode) = (ANode){
        .value = element,
        .next =NULL,
    };

    if(index == 0) {
        array->first = newNode;
    } else {
        ANode *cur = array->first;
        for(int  i =0; i < index; i++) {
            cur = cur->next;
        }
        ANode *next = cur->next;
        cur->next = newNode;
        newNode->next = next;
    }
    array->size +=1;
}
// 设置链表下标的值
void
AList_set(AList *array, long index, void *element){
    assert(index < array->size);
    ANode * cur = array->first;
    for(int i =0; i < index; i++) {
        cur = cur->next;
    }
    cur->value = element;
}

void *
AList_pop(AList *array) {
    assert(array->size > 0);

    ANode *cur = array->first;
    ANode *prev = NULL;

    if(array->size == 1) {
        // 只有一个节点
        void *value = cur->value;
        free(cur);
        cur=NULL;

        array->size = 0;
        array->first = NULL;

        return value;
    }

    // prev 倒数第二个
    // cur 最后一个
    while(cur->next) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = NULL;
    array->size -= 1;

    void * value = cur->value;
    free(cur);
    cur = NULL;

    return value;
}

void
AList_removeAtIndex(AList *array, long index) {
    assert(index < array->size);

    ANode *cur = array->first;
    ANode *prev = NULL;

    long i = 0;
    while(i < array->size) {
        if(i == index) {
            if(i == 0) {
                // 删除链表头
                ANode * new_head = cur->next;
                array->first = new_head;
            } else {
                ANode * next_node = cur->next;
                prev->next = next_node;
            }
            
            cur->next = NULL;
            free(cur);
            cur = NULL;

            array->size -= 1;
            break;
        }

        prev = cur;
        cur = cur->next;
        i++;
    }
}