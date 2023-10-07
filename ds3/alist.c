
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alist.h"

ANode *
ANode_new(void * val, int valType){
    ANode *n = malloc(sizeof(ANode));
    n->dataType = valType;
    if(valType == 0) {
        n->data.n = *(int*)val;
    } else {
        n->data.strVal = (char*)val;
    }

    n->next = NULL;

    return n;
}

AList *
AList_new(void) {
    AList *list = malloc(sizeof(AList));
    list->len = 0;
    list->head = NULL;
    return list;
}
// 添加元素在末尾
//  void * 万能指针，可以指向任何数据类型
void
AList_add(AList *array, void *element) {
    ANode *newNode = (ANode *)element;
    ANode *cur = array->head;

    // 第1次添加
    if(array->len == 0 ) {
        array->head = newNode;
    } else {
        while(cur->next != NULL) {
          cur = cur->next;
        }
        cur->next = newNode;
    }
    array->len += 1;
}

void
AList_print(AList *array) {
    ANode *cur = array->head;
    printf("----start----\n");
    printf("len: %ld \n", array->len);
    while(1) {
        if(cur == NULL) {
            printf("----end--- \n");
            break;
        }
        if(cur->dataType == 0) {
            printf("%d \n", cur->data.n);
        } else {
            printf("%s \n", cur->data.strVal);
        }
        cur = cur -> next;
    }
}

long
AList_length(AList *array) {
    return array->len;
}


void *
AList_get(AList *array, long index) {
    ANode *res = malloc(sizeof(ANode));
    ANode *cur = array->head;
    long len = array->len;
    long i = 0;
    while (cur && i <= index)
    {
        if(i == index) {
            res = cur;
            break;
        }
        i++;
        // 等价于 cur = cur -> next
        cur = (*cur).next;
    }
    return res;
}

// 设置链表下标的值
// t: 0 表示数字， 1 表示字符串
void
AList_set(AList *array, long index, void *element, int t){
    ANode *cur = array->head;
    long len = array->len;
    long i = 0;
    while(cur && i <= index) {
        if(i == index) {
            if(t == 0) {
                cur->data.n = *(int*)element;
                cur->dataType = 0;
            } else {
                cur->data.strVal = (char*)element;
                cur->dataType = 1;
            }
            break;
        }
        i++;
        cur = (*cur).next;
    }
}

void *
AList_pop(AList *array) {
    ANode *cur = array->head;
    ANode *prev = NULL;
    ANode *res = malloc(sizeof(ANode));

    if(array->len == 1) {
        // 只有一个节点
        res = cur;
        free(cur);
        cur=NULL;

        array->len = 0;
        array->head = NULL;

        return res;
    }
    while(cur->next) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = NULL;
    array->len -= 1;

    res = cur;
    free(cur);
    cur = NULL;

    return res;
}

void
AList_removeAtIndex(AList *array, long index) {
    ANode *cur = array->head;
    ANode *prev = NULL;
    long i = 0;
    while(i < array->len) {
        if(i == index) {
            if(i == 0) {
                // 删除链表头
                ANode * new_head = cur->next;
                array->head = new_head;
            } else {
                ANode * next_node = cur->next;
                prev->next = next_node;
            }
            
            cur->next = NULL;
            free(cur);
            cur = NULL;

            array->len -= 1;

            break;
        }
        prev = cur;
        cur = cur->next;
        i++;
    }
}