
#include "../lib/aarray.h"
#include "../lib/astring.h"

/*
data 现在是一个长度为 1 的 AArray * 数组
*/

typedef struct _AMap {
    AArray *data[1];
} AMap;



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