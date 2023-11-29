
#include "aarray.h"
#include "astring.h"

/*
本作业在上个的基础上只改动了 1 点

data 现在是一个指针
AMap_new 的时候给它动态分配内存，长度写死为 101
capacity 存的是 data 的长度
*/

typedef struct _AMap {
    AArray **data;
    unsigned int capacity;
    unsigned int count;
} AMap;

unsigned int
_hash(AString *key);


// 创建一个 AMap
AMap *
AMap_new(void);


// 固定 value 为 AString 类型
void
AMap_set(AMap *m, AString *key, AString *value);


// 返回一个 key 对应的 value, 找不到就返回 NULL
AString *
AMap_get(AMap *m, AString *key);


// 返回是否有这个 key
bool
AMap_has(AMap *m, AString *key);