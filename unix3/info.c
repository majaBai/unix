/*
上一个作业中我们要输出 username groupname
很多同学可能会搜索到 getpwuid 之类的函数

你 man 一下看看就知道，它是在 (3) 也就是标准 C 语言库
Standard C Library (libc, -lc)

这不是系统调用
系统调用没有实现这个 api
那么这个 api 是怎么实现的呢？

实际上是通过对以下两个文件的读取实现的
/etc/passwd
/etc/group
这两个文件存储了所有的用户信息和 group 信息
这也是 linux 的标准之一（实际上或者叫 POSIX(portable OS interface) 标准）
标准不一定要很正规
api 也不一定要是一个函数调用，给你获取信息的方法就是 api 了


自行读取解析这 2 个文件，格式自己感觉
实现下面两个函数
usernameById
groupnameById
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include<pwd.h>
#include <grp.h>
#include<time.h>
#include<sys/types.h>
#include <errno.h>

#include <dirent.h>
#include <stdbool.h>


char *
splitStrbefore(char *str, int n){
    char *res = malloc(sizeof(char) * 128);
    char d = ':';
    int total = strlen(str);
    int cur = 0;
    int k = 0;
    for(int i = 0; i < total; i++){
        char s = str[i];
        if(s == d){
            cur++;
            if(cur == n) {
                break;
            }
            continue;
        }
        if(cur == n - 1) {
            res[k] = s;
            k += 1;
        }
    }
    return res;
}
int
groupnameById(char *id){
    char *path = "/etc/group";
    int found = 0; // 默认没有找到 id 对应的用户
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(0);  // 退出程序
    }
    struct stat sb;
    
    if (stat(path, &sb) == -1) {
        perror("stat获取文件状态信息失败");
        exit(0);
    }
    char *file_contents = malloc(sb.st_size);
    while (fscanf(fp, "%[^\n] ", file_contents) != EOF) {
        printf("%s\n", file_contents);
        char *name = splitStrbefore(file_contents, 1);
        char *g_id = splitStrbefore(file_contents,3);
        // printf("name: %s, g_id: %s\n",name, g_id);
        if(strcmp(id, g_id) == 0) {
            printf("找到id为 %s 的组群: %s\n", id, name);
            found = 1;
            break;
        }
        free(name);
        free(g_id);
    }
    if(found == 0) {
        printf("未找到id为 %s 的组群\n", id);
    }
    fclose(fp);
    
    return 0;
}
int
usernameById(char *id) {
    char *path = "/etc/passwd";
    int found = 0; // 默认没有找到 id 对应的用户
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(0);  // 退出程序
    }
    struct stat sb;
    
    if (stat(path, &sb) == -1) {
        perror("stat获取文件状态信息失败");
        exit(0);
    }
    char *file_contents = malloc(sb.st_size);
    /*
    "%[^\n] "格式字符串被指定为读取文件流，直到遇到新的换行字符。
    fscanf 当到达输入结束时返回 EOF
    下面逐行读取/etc/passwd中的数据，
    每一行代表一个用户
    */
    while (fscanf(fp, "%[^\n] ", file_contents) != EOF) {
        // printf("%s\n", file_contents);
        char *name = splitStrbefore(file_contents, 1);
        char *u_id = splitStrbefore(file_contents,3);
        if(atoi(id) == atoi(u_id)) {
            printf("找到id为 %s 的用户: %s\n", id, name);
            found = 1;
            break;
        }
        free(name);
        free(u_id);
    }
    if(found == 0) {
        printf("未找到id为 %s 的用户\n", id);
    }
    fclose(fp);
    return 0;
}
/*
/etc/passwd 文件每行对应一个用户（很多用户是系统需要的，伪用户）. 例如：
root  :  x :  0 : 0  :  root   : /root : /bin/bash
用户名：密码：UID：GID：描述性信息：主目录：默认Shell
*/
int
main(){
    char *id = "0";
    groupnameById(id);
    usernameById(id);
    return 0;
}