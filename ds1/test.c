#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  "astring.h"

char *
stringCat(char *s1, char *s2) {
    // 求出 s1 s2 的长度
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    // 拼接后的新字符串 s3 的长度要多 1 个用于放末尾的 '\0'
    int len3 = len1 + len2 + 1;
    char *s3 = malloc(sizeof(char) * len3);

    // 循环把 s1 里的字符都复制到 s3 中
    for (int i = 0; i < len1; i++) {
        *(s3 + i) = *(s1 + i);
        // 也可以写为 s3[i] = s1[i]
    }

    // 循环把 s1 里的字符都复制到 s3 后面
    for (int i = 0; i < len2; i++) {
        *(s3 + i + len1) = *(s2 + i);
        // 也可以写为 s3[i+len1] = s2[i]
    }

    // 最后一位要设置为 '\0' 才是一个合格的字符串
    *(s3 + len3 - 1) = '\0';
    // 也可以写为 s3[len3-1] = '\0'

    return s3;
}

void
main() {

    char *t1 = "012";
    char *t2 = " 345";

    char *r = stringCat(t1, t2);
    // printf("r %s, %ld \n", r, strlen(r));
   

    // int i = 0;
    //  while(r[i]) {
    //     i++;
    //  }
    //  printf("i: %d \n", i);


    const char *s1 = "ab0 cdf";
    const char *s2 = "cdfq";
    const char *s3 = "cdf";
    const char *s4 = "f";
    AString *str1  = AString_new(s1);
    AString *str2  = AString_new(s2);
    AString *str3  = AString_new(s3);
    AString *str4  = AString_new(s4);

    printf("****AString_charAt [0]**** : %c \n", AString_charAt(str1, 2));
    printf("****AString_charAt [ ]**** : %c \n", AString_charAt(str1, 3));
    printf("****AString_charAt [f]**** : %c \n", AString_charAt(str4, 0));

    //  printf("****AString_concat [cdff]***: %s \n", AString_concat(str3, str4)->data);
    //  printf("****AString_concat [cdfqcdf]***: %s \n", AString_concat(str2, str3)->data);
    // printf("****AString_find [4]****: %ld \n", AString_find(str1, str3));
    // printf("****AString_find [2]****: %ld \n", AString_find(str2, str4));

    // printf("****endsWith [false]****: %d \n", AString_endsWith(str1, str2));
    // printf("****endsWith [true]****: %d \n", AString_endsWith(str1, str3));

    // printf("str1: %s - %ld\n", str1->data, str1->length);
    // printf("length--: %ld \n", AString_length(str1));
    // printf("****cut1****: %s \n", AString_cut(str1, 0, 3)->data);
    // printf("****cut2****: %s \n", AString_cut(str1, 4,7)->data);
    // printf("equals [false]--: %d \n", AString_equals(str1, str2));
    // printf("equals [true]--: %d \n", AString_equals(str3, str2));
    // printf("startsWith [true]--: %d \n", AString_startsWith(str2, str3));
    // printf("startsWith [false]--: %d \n", AString_startsWith(str1, str2));
    
}

/*
编译 cc test.c astring.c
*/