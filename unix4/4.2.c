/*
然后我们要【执行程序】
unix 执行程序的 api 是 execve
man 2 execve 可以看到函数的声明
 int
 execve(const char *path, char *const argv[], char *const envp[]);

char *const argv[] 与 char **argv 是一样的
path 必须是一个【绝对路径】，你可以使用 /bin/date 测试
argv 是程序的参数，你可以给 NULL
envp 是环境变量，你可以给 NULL


注意，这个调用之后，你的程序就被替换为新程序了
比如你下面两行代码的 log 是不会执行的，因为已经被替换为新进程了
execve()
log(123)


这个作业路径如下
unix/4/2.c

但是你现在会问了， shell 执行程序可不会把自己弄死
所以下个作业我们会学新的调用
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
main(int argc, char **argv){
    // char *p = "/bin/date";
    char *p = "./out/4.1.o";
    char *const args[] = {"", NULL};
    //argv不能为NULL,否报错，至少要传递一个指向包含程序名称的字符串的指针数组。
    // 如果不需要传递任何参数，应该传递一个指向空字符串的指针数组。
    execve(p,args, NULL);
    printf("execve执行后还能打印吗\n");
    return 0;
}