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
    
    AMap * mp = Amap_newWithCap(40 * 10000);
    AString **allStr;
    allStr = (AString **)malloc(40 * 10000 * sizeof(AString *));
    for(int i = 0; i < 40 * 10000; i++) {
        allStr[i] = AString_new("");
    }
    int j = 0; // 一个单词的长度
    int count = 0;
    for(int i = 0; i < file_size;){
        if(allFile[i] == '\n') {
            char *temp =  (char *)malloc(j + 1);
            strncpy(temp, allFile + i - j, j);
            temp[j] = '\0';
            
            // AString * s = AString_new(temp);
            allStr[count]->data = temp;
            AMap_set(mp, allStr[count], NULL);
            j = 0;
            count++;
        }
        j++;
        i++;
    }

    end=clock();
    printf("time= %f s\n",(float)(end-begin)/CLOCKS_PER_SEC);
    printf("count= %d\n",count);

    // 看看 AMap 容量的占用情况
    {
        for(int i = 0; i < mp->capacity; i++) {
            if(mp->data[i] == NULL) {
                continue;
            }
            AArray *d = mp->data[i];
            unsigned int len = AArray_length(d);
            if(len > 0) {
                printf("%u \n",len);
            }
            
        }
    }

    AMap_destroy(mp);
}