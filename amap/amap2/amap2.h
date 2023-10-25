#include "../lib/astring.h"
#include "../lib/aarray.h"
/*
AMap 类里只有一个 data 数组

AMap_set 的时候遍历 data 数组
    1. 如果 data 数组长度是 0  说明我们是第一次存入数据
        创造一个 AArray *kv  把 key value 存入 kv 数组
        把 kv 存入 data 数组
    2. 如果 data 数组长度不是 0
        遍历 data 数组检查 key 是否有了
        有了就覆盖 value
        没有就添加一个新的 kv

AMap_get AMap_has 就遍历 data 数组
目前这个类只实现了 hashtable 的接口，没有实现它的性能
*/

typedef struct _AMap {
    AArray *data;
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