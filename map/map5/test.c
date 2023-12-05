#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "map.h"
/*
在上一个基础上，我们可以预先创建 40w 个 AString，免去在循环中频繁 new 的开销
这里不应该走 AString_new 接口，它一次只申请一小块内存
我们应该手动一次性申请 40w 个 AString 那么大的内存
*/
// #include "../lib/alist.h"

// cc map.c ../lib/*.c test.c -Ilib && ./a.out

// 优化 cc -O3  map.c ../lib/*.c test.c -Ilib && ./a.out
// valgrind 检查内存泄露
// cat r2.txt|sort > 2.sort 将 r2.txt 排序并输入到 2.sort
void
main() {
    int begin,end;
    begin=clock();

     // 一次性读取文件所有内容
    long file_size;
    char *allFile;
    FILE *fp = fopen("../words2.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fseek(fp, 0L, SEEK_END); // 将文件指针移到文件末尾
    file_size = ftell(fp); // 获取文件大小
    rewind(fp); // 将文件指针移到文件开头
    allFile = (char*) malloc(file_size); 
    // 动态分配内存
    if (allFile == NULL) {
        printf("内存分配失败\n");
        exit(2);
    }
    fread(allFile, file_size, 1, fp); // 读取文件内容到内存中
    fclose(fp); // 关闭文件

    printf("读完文件\n");
    AMap * mp = Amap_newWithCap(40 * 10000);
    AString **allStr = (AString **)malloc(40 * 10000 * sizeof(AString*));
    
    int curStr = 0;
    allStr[curStr]->data = allFile;
    allStr[curStr]->length = 0;


    printf("申请40万内存，有问题\n");
    int count = 0;
    for(char * p = allFile; *p != 0; p++) {
        printf("循环\n");
        if(*p == '\n'){
            printf("单词\n");
            *p = 0;
            AMap_set(mp, allStr[curStr], NULL);
            curStr++;
            allStr[curStr]->data = p +1;
            allStr[curStr]->length = 0;
            count++;
        } else {
            allStr[curStr]->length += 1;
        }
    }

    end=clock();
    printf("time= %f s\n",(float)(end-begin)/CLOCKS_PER_SEC);
    printf("count= %d\n",count);


    // 看看 AMap 容量的占用情况
    // {
    //     for(int i = 0; i < mp->capacity; i++) {
    //         if(mp->data[i] == NULL) {
    //             continue;
    //         }
    //         AArray *d = mp->data[i];
    //         unsigned int len = AArray_length(d);
    //         if(len > 0) {
    //             printf("%u \n",len);
    //         }
            
    //     }
    // }

    AMap_destroy(mp);
}