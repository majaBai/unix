#include <stdbool.h>

// typedef 可使用别名 AString
// 在具体代码中，别名前面不用加 struct, 比如 sizeof(AString)， 而不必 sizeof(struct AString)
typedef struct _AString{
    long length;
    char *data;
} AString;

AString *
AString_new(const char *s);

long
AString_length(AString *s);

AString *
AString_cut(AString *s, long start, long end);

bool
AString_equals(AString *s1, AString *s2);

bool
AString_startsWith(AString *s1, AString *s2);

bool
AString_endsWith(AString *s1, AString *s2);

long
AString_find(AString *s1, AString *s2);

AString *
AString_concat(AString *s1, AString *s2);

char
AString_charAt(AString *s, long index);