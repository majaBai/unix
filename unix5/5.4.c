/*
作业目录
unix/5/4.c


有一个名为 SIGWINCH 的信号
当你拖动改变终端尺寸的时候会收到这个信号

man signal | grep SIGWINCH
28    SIGWINCH     discard signal       Window size change


下面是获取终端大小的代码
struct winsize size;
ioctl(0, TIOCGWINSZ, &size);
printf("row(%d) col(%d)\n", size.ws_row, size.ws_col);


写一个程序
main 里面无限循环并且用 sleep(1) 定住程序
它能响应 SIGWINCH 信号并且输出终端尺寸

*/
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int
main(int argc, char** argv, const char* envp) {
    while(1) {
        struct winsize size;
        ioctl(0, TIOCGWINSZ, &size);
        printf("row(%d) col(%d)\n", size.ws_row, size.ws_col);
        sleep(1);
    }
    return 0;
}