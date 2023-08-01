/*
作业 2.1
 实现一个 ls.c 程序
 如果有 一个参数，就把参数当 path
 如果没参数，就用当前目录当 path
 输出 path 下所有的文件的文件名和文件类型（只要求标注是目录还是普通文件）
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include <errno.h>

#include <dirent.h>
#include <stdbool.h>

int
myLs(char *path){
    int numberOfFiles = 0;
    // if(strlen(path) <= 0){
    //     // 默认当前目录
    //    path = getcwd(NULL, 0);
    // } 
    DIR *dir = opendir(path);
    printf("open dir %p\n", dir);
    
    while(true) {
        struct dirent *dirData = readdir(dir);
        if(dirData != NULL) {
            if (dirData->d_type == DT_DIR) {
                printf("dir: ");
            } else {
                printf("file: ");
            }
            numberOfFiles++;
            printf("name：(%s) \n", dirData->d_name);
        } else {
            break;
        }
    }
    printf("files：%d\n", numberOfFiles);

    return 0;
}

int
main(int argc, char **argv) {
    /*
     main 的两个参数
     int argc 表示参数的个数
        程序自己是第一个参数
     const char **argv 是具体的参数
        所以 *argv（argv[0]）里面存的是一个 char *
         也就是说 argv[0] 是第 1 个参数的字符串
         也就是说 argv[1] 是第 2 个参数的字符串
        这不需要理解，知道这么用就好了，慢慢会理解的
    */
   printf("有 %d 个参数 \n", argc);
   printf("第1个参数  %s \n", argv[0]);
   printf("第2个参数  %s \n", argv[1]);
   char *def_path = getcwd(NULL, 0);
   char *path = def_path;
   if(argc > 1) {
     path = argv[1];
   };
    myLs(path);
    free(def_path);
    return 0;
}