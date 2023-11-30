#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "map.h"
/*
现在 map 这边已经很优化了

可以考虑优化一下文件读取，改为一次性读出所有数据，然后遍历去把 \n 改成 \0 并且动态计算出每个字符串的长度
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

    
    AMap * mp = Amap_newWithCap(40 * 1000);

    int j = 0; // 一个单词的长度
    int count = 0;
    for(int i = 0; i < file_size;){
        if(allFile[i] == '\n') {
            char *temp =  (char *)malloc(j + 1);
            strncpy(temp, allFile + i - j, j);
            temp[j] = '\0';
            
            AString * s = AString_new(temp);
            AMap_set(mp, s, NULL);
            j = 0;
            count++;
        }
        j++;
        i++;
    }

    // char *line = NULL;
    // size_t len = 0;
    // ssize_t read;
    // double count;
    // while ((read = getline(&line, &len, fp)) != -1)
    // {
    //     count++;
    //     AString * s = AString_new(line);
    //     AMap_set(mp, s, NULL);
    // }

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