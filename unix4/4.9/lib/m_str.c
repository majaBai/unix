#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *
str_cat(char *s1, char *s2) {
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

/* 
[strat, end] 闭区间
*/
char*
str_sub(char* str, int start, int end){
    int len = end - start + 1;
    char *res = malloc(sizeof(char) * (len + 1));
    for(int i = 0; i < len; i++) {
        res[i] = str[start +  i];
    }
    res[len] = '\0';
    return res;
}

int
str_find(char* str, char *c){
    int i = 0;
    int c_len = strlen(c);
    while(i < strlen(str)) {
        int end = (i + c_len - 1) > (strlen(str) - 1) ? strlen(str) - 1 : i + c_len - 1;
        char* sb_s = str_sub(str, i, end );
        if(strcmp(sb_s, c) == 0){
            return i;
        }
        i += 1;
    }
    return -1;
}

/*
去掉字符串首尾所有空格
*/
char*
str_trim(char *str) {
    int len = strlen(str);
    char *res = malloc(sizeof(char) * (len + 1));
    // 找到开头的空白停止位置
    int i = 0;
    while( i < len) {
        if(str[i] != ' ') {
            break;
        }
        i++;
    }
    if(i == len) {
        // 说明全是空格
        char *emp = "";
        res = emp;
        return res;
    }
    // 找到结尾的空白停止位置
    int j = len - 1;
    while(j >= 0) {
        if(str[j] != ' '){
            break;
        }
        j--;
    }
   
    char *sub = str_sub(str, i, j);
    strcpy(res, sub);
    return res;
}



/*
按照del 分割字符串为数组
*/
char**
str_split(char* str, char* del) {
    char** res = malloc(sizeof(char *) * 64); // 假设结果最多分割64个字符串
    int num = 0;
    int d_len = strlen(del);
    while(strlen(str) > 0) {
        int idx = str_find(str, del);
        if(idx == -1) {
            res[num] = str;
            return res;
        } else if (idx == 0) {
            // 开头
            char *sb_s = str_sub(str, d_len, strlen(str) - 1);
            res[num] = str;
            return res;
        } else if (idx == strlen(str) - 1) {
            // 结尾
            char *sb_s = str_sub(str, 0, strlen(str) - d_len - 1);
            res[num] = str;
            return res;
        } else {
            // 中间
            char *sb_s = str_sub(str, 0, idx -1);
            res[num] = sb_s;
            num +=1;
            str = str_sub(str, idx + d_len, strlen(str) - 1);
        }
    }
    return res;
}