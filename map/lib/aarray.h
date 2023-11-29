typedef struct _AArray {
    unsigned int length;
    unsigned int capacity;
    void **data;
} AArray;


AArray *
AArray_new(void);

void
AArray_destroy(AArray *array);

long
AArray_length(AArray *array);

void *
AArray_get(AArray *array, long index);

void
AArray_set(AArray *array, long index, void *element);

void
AArray_add(AArray *array, void *element);

// 删除并返回末尾的元素
void *
AArray_pop(AArray *array);

void
AArray_removeAtIndex(AArray *array, long index);

void
AArray_insertAtIndex(AArray *array, long index, void *element);