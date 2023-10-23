
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <assert.h>

#include "astring.h"


AString *
AString_new(const char *s) {
    AString *str = malloc(sizeof(AString));
    unsigned long len = strlen(s);
    char *data = malloc(len + 1);
    strncpy(data, s, len);
    data[len] = '\0'; // strncpy 不会复制末尾的 0

    str->length = len;
    str->data = data;
    // int i = 0;
    // while(s[i]) {
    //     i++;
    // };
    // str->length = i;

    // 自己手动复制
    // str->data = malloc(i + 1);
    // int j = 0;
    // while(j < str->length) {
    //    str->data[j] = s[j];
    //     j++;
    // }
    //  str->data[j] = '\0';

    return str;
}

long
AString_length(AString *s){
    return s->length;
}

AString *
AString_cut(AString *s, long start, long end){
    assert(start < s->length);
    assert(end <= s->length);


    long len = end - start;
    char *temp = malloc(len + 1);


    //  data + start 妙啊
    // strncpy 不包含 end
    strncpy(temp, s->data + start, len);
    temp[len] = '\0';

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
    // s1 必须大于等于 s2
    if(s2->length > s1->length) {
        return false;
    }
    // 用 s2 长度遍历
    for(int i = 0; i < s2->length; i++) {
        if(s1->data[i] != s2->data[i]) {
            return false;
        }
    }
    return true;
}

bool
AString_endsWith(AString *s1, AString *s2){
    // s1 长度不能小于 s2
    if(s2->length > s1->length) {
        return false;
    }

    // 用 s2 长度反向遍历
    for(int i = 0; i < s2->length; i++) {
        int idx1 = s1->length - 1 - i;
        int idx2 = s2->length - 1 - i;
        if(s1->data[idx1] != s2->data[idx2]) {
            return false;
        }
    }
    return true;
}

long
AString_find(AString *s1, AString *s2){
    long len = s2->length;

    for(int i = 0 ; i < (s1->length - s2->length + 1); i++) {
        bool found = true;
        for(int j = 0; j < s2->length; j++) {
            int idx1 = i + j;
            int idx2 = j;
            if(s1->data[idx1] != s2->data[idx2]) {
                found = false;
                break;
            }
            if(found == true) {
                return i;
            }
        }
    }
    return -1;

}

AString *
AString_concat(AString *s1, AString *s2){
    long total = s1->length + s2->length;
    char *res = malloc(total + 1);

    // copy s1
    strncpy(res, s1->data, s1->length);
    // copy s2， 算好指针的位置
    strncpy(res + s1->length, s2->data, s2->length);

    res[total] = '\n';

    // 换个写法，不用 new
    AString *as = malloc(sizeof(AString));
    as->data = res;
    as->length = total;

    return as;
}

char
AString_charAt(AString *s, long index){
    if(index >= s->length) {
        return -1;
    }
    return s->data[index];
}

