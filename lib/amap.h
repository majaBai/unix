
#include "aarray.h"
#include "astring.h"

/*
默认的 map 实现会动态扩容，
增加一个 AMap_newWithCapacity API，在创建的时候初始化容量
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

AMap *
Amap_newWithCap(int cap);


// 固定 value 为 AString 类型
void
AMap_set(AMap *m, AString *key, AString *value);


// 返回一个 key 对应的 value, 找不到就返回 NULL
AString *
AMap_get(AMap *m, AString *key);


// 返回是否有这个 key
bool
AMap_has(AMap *m, AString *key);

// 销毁
void
AMap_destroy(AMap* m);