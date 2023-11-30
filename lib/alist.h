#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    void * value;
    struct _Node *next;
} ANode;

typedef struct _AList {
    // 内容自己定
    unsigned int size;
    ANode *first;
} AList;

//flag 区分 dataType
ANode *
ANode_new(void * val);

AList *
AList_new(void);



// 返回链表长度
long
AList_length(AList *array);

// 取出元素
void *
AList_get(AList *array, long index);

// 设置链表下标的值
void
AList_set(AList *array, long index, void *element);

// 添加元素在末尾
void
AList_add(AList *array, void *element);

// 取出并删除末尾的元素
void *
AList_pop(AList *array);

// 按下标删除
void
AList_removeAtIndex(AList *array, long index);

// 按下标插入
void
AList_insertAtIndex(AList *array, long index, void *element);