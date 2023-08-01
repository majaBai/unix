/*
上个作业中的 envp 是【环境变量】
环境变量就是一个变量
我们的程序被别的程序用 execve 启动后，就能接收到这个变量
你可以像打印 argv 一样打印这个 envp

问题是，我们怎么拿到这个 envp？

我们的程序入口是 main 函数
要拿到一个变量，有两个方式

1，它是全局变量，我们知道它的地址（变量名）
2，通过参数传进来


所以如果我们用下面的方式定义 main 函数
就能收到 envp 这个变量 (amazing)
int
main(int argc, const char **argv, const char **envp)



或者你用下面方式声明一个全局变量 (unbelievable)
extern char **environ;

extern 的意思是说，这个变量在链接的时候会被赋值的
不要为这个变量申请内存

因为我们的程序是被别的程序 execve 执行的
所以我们的 main 函数执行的时候， environ 已经被设置好了
可以直接使用

这两个方式是一样的

有兴趣可以看下面的文档
https://man7.org/linux/man-pages/man7/environ.7.html

作业路径
unix/4/3.c

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

int
main(int argc, char **argv, char **envp){
    printf("environ in 4.3: %s \n", envp[0]);
    char *p = "./out/4.1.o";
    char *const args[] = {"", NULL};
    execve(p,args, envp);
    printf("execve执行后还能打印吗\n"); // 不会打印
    return 0;
}