#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


/*
作业目录
unix/5/1.c

作业描述（看完你就会写了）
./a.out PID n

比如下面的用法给 pid=100 的进程发 SIGINT
./a.out 100 2




shell 在运行程序的时候可以做很多事情

比如我们可以在运行本程序的时候
输入 Ctrl-c 来中断程序

这是通过一种叫 signal（信号）的机制实现的


每个进程都可以给别的进程发信号
我们输入 Ctrl-c 的时候，系统发送了一个 SIGINT 信号

SIGINT 其实就是数字 2 的宏定义
应该是这样实现的
#define SIGINT 2



下面是信号列表和数字定义（但是你不用关心）
发出来就是让你知道这个不重要的

Signal        x86/ARM     Alpha/   MIPS   PARISC   Notes
           most others   SPARC
─────────────────────────────────────────────────────────────────
SIGHUP           1           1       1       1
SIGINT           2           2       2       2
SIGQUIT          3           3       3       3
SIGILL           4           4       4       4
SIGTRAP          5           5       5       5
SIGABRT          6           6       6       6
SIGIOT           6           6       6       6
SIGBUS           7          10      10      10
SIGEMT           -           7       7      -
SIGFPE           8           8       8       8
SIGKILL          9           9       9       9
SIGUSR1         10          30      16      16
SIGSEGV         11          11      11      11
SIGUSR2         12          31      17      17
SIGPIPE         13          13      13      13
SIGALRM         14          14      14      14
SIGTERM         15          15      15      15
SIGSTKFLT       16          -       -        7
SIGCHLD         17          20      18      18
SIGCLD           -          -       18      -
SIGCONT         18          19      25      26
SIGSTOP         19          17      23      24
SIGTSTP         20          18      24      25
SIGTTIN         21          21      26      27
SIGTTOU         22          22      27      28
SIGURG          23          16      21      29
SIGXCPU         24          24      30      12
SIGXFSZ         25          25      31      30
SIGVTALRM       26          26      28      20
SIGPROF         27          27      29      21
SIGWINCH        28          28      20      23
SIGIO           29          23      22      22
SIGPOLL                                            Same as SIGIO
SIGPWR          30         29/-     19      19
SIGINFO          -         29/-     -       -
SIGLOST          -         -/29     -       -
SIGSYS          31          12      12      31
SIGUNUSED       31          -       -       31

Note the following:
*  Where defined, SIGUNUSED is synonymous with SIGSYS.  Since
  glibc 2.26, SIGUNUSED is no longer defined on any
  architecture.
*  Signal 29 is SIGINFO/SIGPWR (synonyms for the same value) on
  Alpha but SIGLOST on SPARC.



进程收到信号后，如果没有响应，则会触发默认行为
绝大部分信号的默认行为是【退出进程】

Ctrl-c 能对前台进程发 SIGINT 信号



也可以用 man 2 kill 这个系统调用来发送信号（要自己写个程序）

shell 中也有个内置命令 kill （这个程序太简单所以被 shell 内置了）
下面的命令就可以对 [PID] 这个进程发一个信号 2 （）
kill -2 [PID]

这个程序会打印自己的 pid 方便你 kill



man 2 sigaction 这个调用可以让我们生成一个对信号的响应
信号被响应了就不会触发默认行为了

但是 9（SIGKILL) 这个信号是不能响应的
所以 kill -9 一定能杀掉进程

sigaction  是处理别人发的信号
kill 是给别人发信号
*/
void
handler() {
    // signal handler 里面是不应该使用 printf 这样的函数的
    // 原因是 signal 随时可能到来
    // printf 执行到一半还没返回的时候就可能被信号打断
    // 但现在为了方便我们就这样临时用用
    // 一定要记住这里面不能用 printf

    printf("signal\n");
}

int
main(int argc, char **argv) {
    if (argc > 1) {
        // printf("argv[1] %s \n", argv[1]);
        if (strcmp(argv[1], "-sig") == 0) {
            /*
            用 = {0} 的方式给结构体赋值
            可以保证结构体的数据都被初始化为 0
            */
            struct sigaction sig = {0};
            sig.sa_handler = handler;

            /*
            sigaction(int signum, const struct sigaction *act, struct sigaction *oldact )
            signum 对应的响应是 act

            struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
            */

            sigaction(SIGINT, &sig, NULL);
        } else {
            // 比如下面的用法给 pid=100 的进程发 SIGINT
            // ./a.out 100 2
            int pid = atoi(argv[1]);
            int signum = atoi(argv[2]);
            printf("pid: %d , signum: %d \n",pid,signum);
            kill(pid, signum);
        }
    }

    // sleep 100s
    for (int i = 0; i < 10; i++) {
        // 打印 pid 方便你 kill
        // man 2 getpid
        printf("[PID] %d  sleep 10s\n", getpid());
        sleep(10);

        // 你会发现你发送信号后 sleep 就结束了
        // 是的，它被信号打断了
    }

    return 0;
}

/*
执行 ./a.out，可以看到 pid

重开一个终端执行, 执行./a.out pid 2， 上面那个程序就会被 kill

*/