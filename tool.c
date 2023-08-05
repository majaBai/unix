#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

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

// 从 stdin 逐字节读取 max_len 长度的内容
char *
cmdFromStdin() {
    int max_len = 10;
    char *command = malloc(sizeof(char) * (max_len + 1));
    int i = 0;
    char c;
    while(i < max_len && read(0, &c, 1) == 1) {
        if(c == '\n'){
            break;
        }
        command[i] = c;
        i++;
    }
    command[i] = '\0';
    printf("cmd: %s %zd字节\n", command, strlen(command));
    return command;
}