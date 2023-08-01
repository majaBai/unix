/*
我们接下来要贯穿始终的一个大项目是【实现一个全功能的终端，一个全功能的 shell】

实现这两个东西的好处是我们可以对 unix 的这些系统调用有充分深入的了解

我们会逐步拆解它们的实现
先是 shell 再是终端

shell 就是读取命令，然后执行命令

现在我们要实现的是【读取命令】

在 while 循环中使用 read 来读取用户输入的命令
逐字符读取，读到回车符代表接受到了一个命令
我们规定命令的最长长度为 10 个字符

先把收到的命令打印出来
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include<pwd.h>
#include <grp.h>
#include<sys/types.h>

#include <dirent.h>
#include <stdbool.h>

extern char **environ;
int
main(int argc, char **argv, char **envp){
     printf("environ in 4.1: %s \n", environ[0]);
    int max_len = 10;
    char command[max_len + 1];
    printf("请输入指令（不超过10字节）\n");
    while(1){
        int i = 0;
        char c;
        /*
        read(fp, buf, count)
        fp: 要读取的文件，标准输入为0
        buf: 存放读取到的内容
        count：要读取多少字节

        返回值是读取到的字节数
        */
        while(i < max_len && read(0, &c, 1) == 1) {
            if(c == '\n'){
                break;
            }
            command[i] = c;
            i++;
        }

        command[i] = '\0';
        if(command[0] == '\0'){
            break;
        }
        printf("收到的指令：%s\n", command);
    }
    return 0;
}