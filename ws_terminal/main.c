#define _XOPEN_SOURCE 600 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <errno.h>
#include <limits.h>
#include <pthread.h>


#include "ws.h"



// 全局变量
static int _ptm = 0;



//#define ExitIfError(NUM, MSG) {\
//    if(NUM < 0) { \
//        fprintf(stderr, "%s  (%s)\n", ); \
//        exit(-1); \
//    }\
//}

void
AbortIfError(int NUM, const char *MSG) {
    if(NUM < 0) {
        fprintf(stderr, "%s  (%s)\n", MSG, strerror(errno));
        exit(-1);
    }
}

pid_t
SH_forkpty(int *master, int *slave) {
    // 开一对 master slave
    int ptm = open("/dev/ptmx", O_RDWR);
    printf("forkpty ptm %d\n", ptm);
    AbortIfError(ptm, "open /dev/ptmx");
   
    
    // grantpt unlockpt 是套路
    // ptsname 拿到 slave 对应的 pts 路径
    int r1 = grantpt(ptm);
    int r2 = unlockpt(ptm);
    char *devPath = ptsname(ptm);
    printf("ptsname %s", devPath);
    AbortIfError(r1, "open pty");
    AbortIfError(r2, "open pty");
    // ptsname 错误的时候会返回 NULL （也就是 0，但 0 在其他 api 用于表示成功）
    // 这个垃圾 api 让程序写起来烦死了
    // 手动包一下 r3
    int r3 = devPath == NULL ? -1 : 0;
    AbortIfError(r3, "open pty");


    // 打开那个 pts slave
    int pts = open(devPath, O_RDWR);
    AbortIfError(pts, "open ptys");
        
    
    pid_t pid = fork();
    AbortIfError(pts, "fork error");

    if (pid == 0) {
        // 子进程
        
        // slave 要关闭 ptm
        close(ptm);

        
        // setsid 和 ioctl 必须一起调用
        // setsid 是 session leader
        // TIOCSCTTY 是 become controlling tty
        //      必须是 session leader 才能 become controlling tty
        setsid();
        // TIOCSCTTY 全称应该是 Terminal IO Control Set Controlling TTY
        int r = ioctl(pts, TIOCSCTTY, 0);
        AbortIfError(r, "TIOCSCTTY error");
        
        // 让 0 1 2 都是 pts 这个 fd
        dup2(pts, STDIN_FILENO);
        dup2(pts, STDOUT_FILENO);
        dup2(pts, STDERR_FILENO);
        
        // 因为现在 012 都是 pts 了
        // 所以关闭老的 pts
        // 要判断 > 2 是因为当初打开的时候返回的可能是 0 1 2
        //  （为什么可能是 012 呢，因为可能被人 close 了再来 forkpty）
        // 如果这里无脑关，就关错了
        if (pts > 2) {
            close(pts);
        }
    } else {
        // 父进程
        *master = ptm;
        if (slave) {
            *slave = pts;
        }
    }
    return pid;
}

void *
runRead(void *args) {
    ws_cli_conn_t *client = (ws_cli_conn_t *)args;
    while (true) {
        // 就临时写死用 100 的 buffer 吧
        char buf[100] = {};
        const int len = 100;
        size_t n = read(_ptm, buf, len-1);
        buf[n] = '\0';
        printf("main 从 ptm（%d） 读取到 （%s）\n", _ptm, buf);
        
        printf("send ws (%p)\n", client);
        // 用 ws 发给前端
        // ws_sendframe_txt(client, buf);
        ws_sendframe_bin(client, buf, n);
    }
}

void
openTerminal(ws_cli_conn_t *client) {
    /*
     forkpty 是一个特殊的 fork
     它会创建一对进程
     
     父进程是 master
     子进程是 slave
     
     ptm 是一个文件描述符
     master 写入到 ptm 的内容 slave 可以从 0 读到
     slave 写入 1 2 的内容 master 可以从 ptm 中读到
     
     pty_name 是新创建的伪终端的完整路径
    */


    printf("open ws (%p)\n", client);

    int pts;
    pid_t pid = SH_forkpty(&_ptm, &pts);
    AbortIfError(pid, "fork error");

    if (pid == 0) {
        // pid 0 表示子进程
        setenv("TERM", "vt100", 1);

        // 子进程要开一个 shell
        // 这里假设用户的默认 shell 是 zsh
        // 实际上应该用 api 读到用户的默认 shell（懒得找了）
        char *args[]= {
            "/bin/zsh",
            NULL,
        };
        execvp(args[0], args);
    } else {
        // 父进程创建一个线程来读取 ptm 里面的东西
        // 这就是 shell 返回的数据
        pthread_t tid;
        int err = pthread_create(&tid, NULL, &runRead, client);
    }
}
/**
 * @brief This function is called whenever a new connection is opened.
 * @param client Client connection.
 */
void
onopen(ws_cli_conn_t *client) {
    // 为了方便现在只允许一个连接  第二个连接上来就退出
    static bool firstClient = true;
    if(!firstClient) {
        exit(1);
    }
    
    firstClient = false;
    char *cli = ws_getaddress(client);
    printf("Connection opened, addr: %s\n", cli);
    
    // 在这里 forkpty
    
    openTerminal(client);
}

/**
 * @brief Message events goes here.
 * @param client Client connection.
 * @param msg    Message content.
 * @param size   Message size.
 * @param type   Message type.
 */
void
onmessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size, int type) {
    printf("recv msg: (%s)  size(%lu)\n", msg, size);
    
    // 读到的数据就是用户的输入  写入 ptm 就等于传给了 shell
    write(_ptm, msg, size);
}

/**
 * @brief This function is called whenever a connection is closed.
 * @param client Client connection.
 */
void
onclose(ws_cli_conn_t *client) {
    char *cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
}



int
main(int argc, char **argv) {
    /* Register events. */
    struct ws_events evs;
    evs.onopen    = &onopen;
    evs.onclose   = &onclose;
    evs.onmessage = &onmessage;

    /*
     * Main loop, this function never* returns.
     *
     * *If the third argument is != 0, a new thread is created
     * to handle new connections.
     */
    ws_socket(&evs, 23333, 0, 1000);

    
    return 0;
}
