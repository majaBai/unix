
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// typedef 可使用别名 AString
// 在具体代码中，别名前面不用加 struct, 比如 sizeof(AString)， 而不必 sizeof(struct AString)
typedef struct _AString{
    long length;
    char *data;
} AString;

AString *
AString_new(const char *s) {
    AString *str = malloc(sizeof(AString));

    int i = 0;
    while(s[i]) {
        i++;
    };
    str->length = i;

    str->data = malloc(i + 1);
    int j = 0;
    while(j < str->length) {
       str->data[j] = s[j];
        j++;
    }
     str->data[j] = '\0';
    return str;
}

long
AString_length(AString *s){
    return s->length;
}

AString *
AString_cut(AString *s, long start, long end){
    char *data = s->data;
    char *temp = malloc(end - start + 1);
    long i = start;
    while(i < end) {
        temp[i - start] = data[i];
        i++;
    }
    temp[i - start] = '\0';
    AString *res = AString_new((const char*)temp);

    free(temp);
    temp = NULL;

    return res;
}

bool
AString_equals(AString *s1, AString *s2){
    if(s1->length != s2->length) {
        return false;
    }
    for(long i = 0; i < s1->length; i++) {
        if(s1->data[i] != s2->data[i]) {
            return false;
        }
    }
    return true;
}

bool
AString_startsWith(AString *s1, AString *s2){
    if(s2->length > s1->length) {
        return AString_startsWith(s2, s1);
    }
    AString *temp = AString_cut(s1, 0, s2->length);
    bool b = AString_equals(s2, temp);

    free(temp);
    temp = NULL;
    return b;
}

bool
AString_endsWith(AString *s1, AString *s2){
    if(s2->length > s1->length) {
        return AString_endsWith(s2, s1);
    }

    AString *temp = AString_cut(s1, (s1->length - s2->length), s1->length);

    bool b = AString_equals(temp, s2);

    free(temp);
    temp = NULL;

    return b;
}

long
AString_find(AString *s1, AString *s2){
    long len = s2->length;
    int r = -1;
    for(int i = 0; i < s1->length; i += 1){
        long end = i + len >= s1->length ? s1->length : i + len;
        AString *temp = AString_cut(s1, i, end);
        if(AString_equals(temp, s2)) {
            r = i;
            free(temp);
            temp = NULL;
            break;
        }
        free(temp);
        temp = NULL;
    }
    return r;
}

AString *
AString_concat(AString *s1, AString *s2){
    long total = s1->length + s2->length;
    char *temp = malloc(total + 1);
    int i = 0;

    char *str1 = s1->data;
    while(str1[i]){
        temp[i] = str1[i];
        i++;
    }

    free(str1);
    str1 = NULL;

    printf("concat s1: %s, i: %d \n", temp, i);

    char *str2 = s2->data;
    int j = 0;
    while(str2[j]){
        temp[i + j] = str2[j];
        j++;
    }
    free(str2);
    str2 = NULL;

    temp[i + j] = '\0';

    printf("concat s2: %s, j: %d \n", temp, j);

    AString *res = AString_new((const char*)temp);

    free(temp);
    temp = NULL;

    return res;
}

char
AString_charAt(AString *s, long index){
    if(index >= s->length) {
        return -1;
    }
    return s->data[index];
}

