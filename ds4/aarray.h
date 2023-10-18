typedef struct _AArray {
    long len;
    void **base;
} AArray;


AArray *
AArray_new(void);

void
AArray_destroy(AArray *array);

// 返回链表长度
long
AArray_length(AArray *array);

// 取出元素
void *
AArray_get(AArray *array, long index);

// 设置链表下标的值
void
AArray_set(AArray *array, long index, void *element);

// 添加元素在末尾
void
AArray_add(AArray *array, void *element);

// 取出并删除末尾的元素
void *
AArray_pop(AArray *array);

// 按下标删除
void
AArray_removeAtIndex(AArray *array, long index);

// 按下标插入
void
AArray_insertAtIndex(AArray *array, long index, void *element);