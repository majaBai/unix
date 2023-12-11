/*
加入 sleep 后，用 wrk 测试可以发现 qps 下降了上百倍

这是因为我们目前的程序是 accept 后要 sleep 再 write close
然后才能处理第二个请求

sleep 期间的时间，明明 cpu 资源有，但是却没办法处理，这是极大的浪费


我们可以用【多进程模型】来解决这个问题

我们的思路是：
1，在 accept 后拿到 clientSocket
2，fork 一个子进程来处理请求
3，父进程继续下一次循环来 accept 处理请求



这种编程模式会遇到如下几个问题

1，clientSocket 父子进程都有一份
    两个都关闭了才会真正释放这个链接
    想想 fork 作业就能明白这一点


2，子进程退出后，因为父进程没有 wait 它，所以它会成为僵尸进程
    僵尸进程的危害在于用光了系统的 pid，系统没办法再 fork 了

    子进程退出后系统会给父进程发 SIGCHLD 信号
    我们可以简单地 SIG_IGN 这个信号来防止僵尸进程
    但是在 90 年到 01 年间，这个方法不一定有用
    写完全部作业，可以知道为什么
    还可以知道还有什么别的办法，以及有什么问题，和该如何解决（很难在书上看到这些）


3，还有其他小问题，不过都不是问题，这里不描述
    遇到了想看可以去 chat 看
    接下来的其他作业会介绍
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include <time.h>



int
openSocket(unsigned short port) {
        // 这是打开一个 socket 的标准套路
        // 对于这整个函数来说，你只应该复制粘贴
        // 因为 api 设计太失败，才会暴露这么多垃圾概念 数据 接口给你
        // 所以这些东西只配被复制粘贴
        // 你如果想去理解，只能说明你崇拜这一套
        // 你认为专家必有深意
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    {
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(port);
    }

    bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

        // 这个数字 5 叫 backlog
        // 意思是一个连接，在三次握手建立后，会被存放到一个队列中
        // 然后程序调用 accept（下面 main 里面有）后会从队列中读取并移除连接
        // 这个队列的长度叫 backlog
        // 这是一个不应该随意暴露的 api
        // 因为对普通程序而言真的没啥意义
    listen(s, 5);
    //
    printf("listen port (%d)\n", port);
    return s;
}


int
main(void) {
    unsigned short port = 3000;
    int s = openSocket(port);


    struct sockaddr_in client;
    int size = sizeof(struct sockaddr_in);
    while(true) {
            // accept 会卡在这里，直到有客户端的连接请求过来了才会返回
        int clientSocket = accept(s, (struct sockaddr *)&client, (socklen_t *)&size);

        int pid = fork();
        if(pid == 0) {
            char *msg = "HTTP/1.1 200\r\nContent-Type: text/plain\r\nConnection: close\r\nContent-Length: 2\r\n\r\nok";
            write(clientSocket, msg, strlen(msg));
            close(clientSocket);
        }

        // 响应一个请求后sleep
        struct timespec time1;
        time1.tv_sec = 1;
        time1.tv_nsec = 200000000;
        nanosleep(&time1, NULL);

        // 关闭连接
        close(clientSocket);
    }    
    return 0;
}