/*
words2.txt 是一个字典文件
里面有 36w 个单词，每行一个

用 c 语言解析这个文件，生成一个 astring 数组
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../lib/aarray.h"
#include "../lib/astring.h"

// cc -g main.c ../lib/*.c  -Ilib && ./a.out
// valgrind ./a.out 分析内存问题
void
main() {
    clock_t start,end;
    start = clock();
    double time_used;

    FILE *fp = fopen("./words2.txt", "r");
    AArray *arr = AArray_new();
    char data[256] = {0};
	while(!feof(fp))
	{
		fgets(data, sizeof(data), fp);
		// printf("%s \n", data);
        AString * s = AString_new((const char *) data);
        AArray_add(arr, s);
	}
    fclose(fp);
    printf("length: %lu \n", AArray_length(arr));

    end = clock();
    time_used = (double)(end-start)/CLOCKS_PER_SEC;
    printf("time: %f \n", time_used);

    for(int i =0; i < AArray_length(arr); i++){
        AString * s = AArray_get(arr, i);
        AString_destroy(s);
    }
    AArray_destroy(arr);
}