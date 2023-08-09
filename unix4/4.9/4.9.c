/*
利用 pipe fcntl fork 来实现之前作用中 mkfifo 实现的管道功能
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern char **environ;

void
fileControl(void) {
    /*
     fcntl 是一个功能非常复杂的调用

     man 2 fcntl 可以看到原型是下面这样
     int fcntl(int fildes, int cmd, ...);

     三个点 ... 表示这是一个【可变长度参数列表】
     比如 printf 的参数就是可变的

     open 的参数也是可变的
     当第二个参数没有 O_CREAT 的时候，第三个文件权限参数是不必须的


     fcntl 用于控制【文件描述符（fd）】
     参数 1 是要被控制的 fd
     参数 2 是【控制命令（cmd）】
        cmd 为 F_DUPFD 的时候，表示要复制 fd，函数返回值是一个新的描述符，它是 fd 的复制，两个描述符等价
        我们目前只学 F_DUPFD 这一个 cmd
        F_DUPFD 应该是 duplicate file descriptor 的意思
        这种命名是很糟糕的，没规律，没有一致性，且很难不懂缩写（外国人也是看不懂的，外国人也是人）
     参数 3 是对 cmd 的补充参数，我们称之为 arg
        cmd 为 F_DUPFD 的时候，arg 表示要复制出的新描述符该怎么生成
        返回的新描述符是【大于或等于 arg】并且【最小的可用】的描述符
        假设一个进程，已经打开了 0 1 2
            fcntl(1, F_DUPFD, 0) 返回 3
            fcntl(1, F_DUPFD, 1) 返回 3
            fcntl(1, F_DUPFD, 3) 返回 3
            fcntl(1, F_DUPFD, 100) 返回 100

     cmd=F_DUPFD 的时候，arg 可以省略（相当于传 0）
     所以下面两个调用是等价的
     int fd = fcntl(1, F_DUPFD);
     int fd = fcntl(1, F_DUPFD, 0);

    */

    fprintf(stderr, "===\n");
    fprintf(stderr, "[file control]\n");
    fprintf(stderr, "===\n");
    // 下面调用后， 100 就被复制为 1 了
    int fd1 = fcntl(1, F_DUPFD, 100); 
    printf("fd fcntl dup %d\n", fd1);
    // 100 和 1 是等价的
    // 所以 write 100 和 write 1 是一样的
    write(100, "100 writ\n", 9);

    // 下面调用后， fd2 就被复制为 1 了
    // fd2 在这里应该是 3
    // 因为 0 1 2 已经打开了
    int fd2 = fcntl(1, F_DUPFD);
    printf("fd fcntl dup %d\n", fd2);
    // fd2 和 1 是等价的
    // 所以 write fd2 和 write 1 是一样的
    write(fd2, "fd2 write\n", 10);

    // 不用了就可以关闭 fd
    close(fd1);
    close(fd2);

    // 已经关闭了，再写就是错误了
    write(fd1, "", 1);
    perror("[write error]");
}

/*
 我们前面的作业使用了 mkfifo 来实现管道
 fifo 很方便，由于有名字，所以又叫做【命名管道】

 但是还有一种叫做【匿名管道】的东西
 功能虽然少，但是毕竟存在，得学

 这个系统调用是 pipe

 下面是 man 2 pipe 的描述
 不要使用全文翻译，我已经做了简单注解，还有不懂的单词就去查出来
 The pipe() function creates a pipe and allocates（申请） a pair of file descriptors.

 The first descriptor connects to the 【read end】 of the pipe;
 the second connects to the 【write end】.

 Data written to fildes[1] appears（出现） on (i.e.（拉丁文的 in other words）, can be read from) fildes[0].
*/
void
pipe1(void) {
    fprintf(stderr, "\n===\n");
    fprintf(stderr, "[pipe 1]\n");
    fprintf(stderr, "===\n");

    // pipe 会打开 2 个文件描述符
    int fileIds[2];
    pipe(fileIds);
    // 0 1 2 是已经被打开了的，所以这里输出是 3 4
    printf("pipe1 FD (%d), (%d)", fileIds[0], fileIds[1]);


    //    简单的管道代码
    // fileIds[1] 是 【write end】
    const int len = 3;
    write(fileIds[1], "axe", len);

    char buffer[len];
    // fileIds[0] 是 【read end】
    read(fileIds[0], buffer, len);

    // 把读到的东西打印出来
    write(STDOUT_FILENO, "\n", 1);
    write(STDOUT_FILENO, buffer, len);
    write(STDOUT_FILENO, "\n", 1);
}

void
pipe2(void) {
    fprintf(stderr, "\n===\n");
    fprintf(stderr, "[pipe 2]\n");
    fprintf(stderr, "===\n");
    // pipe2 演示了 pipe fork 进行 2 个进程的通信
    int fileIds[2];
    pipe(fileIds);
    // 0 1 2 是已经被打开了的
    // pipe1 打开 3 4
    // 所以这里是 5 6
    printf("pipe2 FD (%d), (%d)", fileIds[0], fileIds[1]);

    pid_t pid = fork();
    const int len = 3;
    if (pid == 0) {
        char buffer[10];
        read(fileIds[0], buffer, len);

        write(1, "\n", 1);
        write(1, "\n", 1);
        write(1, buffer, len);
        write(1, "\n", 1);
    } else {
        write(fileIds[1], "AXE", len); 
    }
}


char *pipe_p;
int fileIds[2];
/*
用匿名管道 pipe 实现命名管道
*/
void
my_mkfifo(char* p, int mode){
    pipe_p = p;
    pipe(fileIds);
    printf("my fifo: %d %d \n", fileIds[0], fileIds[1]);
}

int
my_open(char* path, int flags) {
    if (strcmp(path, pipe_p) == 0) {
        // 打开的是pipe
        if(flags == O_RDONLY) {
            int fd1 = fcntl(fileIds[0], F_DUPFD, STDIN_FILENO); 
            fprintf(stderr, "fcntl read: %d %d \n", fileIds[0], fd1);
            return fd1;
        } else if(flags == O_WRONLY){
            // 将程序输出变成pipe的写入端
            int fd1 = fcntl(fileIds[1], F_DUPFD, STDOUT_FILENO);
            fprintf(stderr, "fcntl write %d %d \n",fileIds[1], fd1);
            return fd1;
        }
    } else {
        // 普通打开，调用系统的 open() 函数
        return open(path, flags);
    }
}
int
main() {
    // fileControl();
    // pipe1();
    // pipe2();
    // char *path = "/tmp/axe.fifo";
    // my_mkfifo(path, 0777);

    int fileIds[2];
    pipe(fileIds);
    printf("my fifo: %d %d \n", fileIds[0], fileIds[1]);

    pid_t pid = fork();
    if (pid == 0){
        close(1);
        // int fd = my_open(path, O_WRONLY);
        int fd = fcntl(fileIds[1], F_DUPFD, STDOUT_FILENO);
        fprintf(stderr, "[child FD] %d\n", fd);
        char *argv[] = {
            "/bin/date",
            NULL};
        execve(argv[0], argv, environ);
    } else {
        close(0);
        // int fd = my_open(path, O_RDONLY);
        int fd = fcntl(fileIds[0], F_DUPFD, STDIN_FILENO); 
        printf("[parent FD] %d\n", fd);
        char **argv = malloc(sizeof(char *) * 2);
        char *path = "/tmp/read.axe";
        argv[0] = path;
        argv[1] = NULL;
        execve(argv[0], argv, environ);
    }
    return 0;
}

