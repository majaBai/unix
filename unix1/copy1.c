/*
作业 1
实现 copy1.c 程序，它的功能如下
1，生成一个长度为 1M = 1024 KB = 1024 * 1024 字节的随机文件 1.source
2，把这个 1.source 文件用 read 逐字节都出来并逐字节写入 1.target
3，read 还没学，所以群里讨论吧
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void
create_source(void) {
    const char *path = "1.source";
    int fileId = open(path, O_CREAT | O_RDWR, S_IRWXU);

    system("pwd");
    system("ls -l");

    // 文件内容 1MB == 1024 KB == 1024 * 1024 Byte
    int length = 1024 * 1024;
    char content[length];
    for(int i = 0; i < length; i ++){
        content[i] = 'a';
    };
    content[length] = '\0';
    size_t len = strlen(content);

    // 写入
    write(fileId, content, len);
    close(fileId);
}

/* 文件大小必须在 2G 以下*/
int
file_size(char* filename){
    FILE *fp = fopen(filename,"r"); 
    if(!fp) { 
        printf("fopen error");
        return -1; 
    };
    fseek(fp,0L,SEEK_END); 
    int size = ftell(fp); 
    fclose(fp); 
    return size;
}

void
copy2(void){
    FILE *source_file, *target_file;
    char buffer[1];
    size_t bytes_read;

    // 打开源文件
    source_file = fopen("1.source", "rb");

    // 打开目标文件
    target_file = fopen("1.target", "w+b");

    // 读取源文件的内容，并将其写入目标文件中
    /*
    fread(*ptr, size_t size, size_t count, file)
    ptr: 用来存放读取到的数据
    size: 每个数据块的字节数（按数据块读取）
    count: 读取多少数据块
    每次读取到的数据大小为 size * count
    file: 从哪个文件读取
    */ 
    while ((bytes_read = fread(buffer, 1, 1, source_file)) > 0) {
        fwrite(buffer, 1, bytes_read, target_file);
    }

    // 关闭文件
    fclose(source_file);
    fclose(target_file);

    printf("文件复制成功 \n");
};

void
copy(void){
    // 源文件
    const char *source_file = "1.source";
    int sourceId = open(source_file, O_CREAT | O_RDWR, S_IRWXU);

    // 目标文件
    const char *target_file = "1.target";
    int targetId = open(target_file, O_CREAT | O_RDWR, S_IRWXU);

    // 读 + 写
    char buf[1];
    size_t bytes_read;

    while((bytes_read = read(sourceId, buf, 1)) > 0){
        write(targetId, buf, 1);
    }

    close(sourceId);
    close(targetId);
    printf("文件复制成功: \n");
}

int
main(void) {
    // create_source();
   copy();

    char *p = "1.source";
    int size = file_size(p);
    printf("1.source size: %d \n", size);

    char *p2 = "1.target";
    int size2 = file_size(p2);
    printf("1.target size: %d \n", size2);
    return 0;
}

