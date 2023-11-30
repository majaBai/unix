#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "map.h"
/*
让 _hash 函数返回递增的下标，确保数据是平铺在 map 里的
这样可以排除 _hash 的干扰，去找到别的瓶颈（最后再来优化 _hash）
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

     // 打开文件
    FILE *fp = fopen("../words2.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // AArray *arr = AArray_new();
    // AList * lt = AList_new();
    // AMap * mp = AMap_new();
    AMap * mp = Amap_newWithCap(40 * 1000);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    double count;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        count++;
        AString * s = AString_new(line);
        // AArray_add(arr, s);
        // AList_add(lt, s);
        // AList_insertAtIndex(lt, 0, s);
        AMap_set(mp, s, NULL);
    }

    end=clock();
    printf("time= %f s\n",(float)(end-begin)/CLOCKS_PER_SEC);

    // printf("count= %f \narr len= %ld \n", count, AArray_length(arr));
    
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

    // 关闭文件
    fclose(fp);

    AMap_destroy(mp);
    // AArray_destroy(arr);
}