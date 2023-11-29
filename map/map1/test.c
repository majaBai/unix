#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "map.h"
// #include "../lib/alist.h"

// cc map.c ../lib/*.c test.c -Ilib && ./a.out

// 优化 cc -O3  map.c ../lib/*.c test.c -Ilib && ./a.out
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

    AMap * mp = AMap_new();
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    double count;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        count++;
        AString * s = AString_new(line);
        // printf("%f , %s\n", count, s->data);
        // AArray_add(arr, s);
        // AList_add(lt, s);
        // AList_insertAtIndex(lt, 0, s);
        AMap_set(mp, s, NULL);
    }

    end=clock();
    printf("time= %f s\n",(float)(end-begin)/CLOCKS_PER_SEC);

    // printf("count= %f \narr len= %ld \n", count, AArray_length(arr));

    // 关闭文件
    fclose(fp);
}