/*
本次作业要给 server 加上一个功能
每个请求 sleep 1ms - 100ms

这是用来模拟读取数据库耗时
大部分 web 业务都是这样的模式
这被称之为 【io 密集】

之后还会介绍 【cpu 密集】的问题

下面给了一个 nanosleep 的例子
// 下面用 3 个方法演示了 nanosleep 的用法
    // 都是实现 sleep 1.2s 的功能的
    // 1s = 1,000ms = 1,000,000us = 1,000,000,000ns


    // 方法 1
    // 书上的原始方法
    {
        struct timespec time1;
        time1.tv_sec = 1;
        time1.tv_nsec = 200000000;
        // 通常会要求在数字的后面加上 L 来表示这是一个 long 类型
        // long 在 64 位系统中是 8 字节
        // 2000000000L
        nanosleep(&time1, NULL);
    }

    // 方法 2
    // 这是新的 C 语言语法
    // 很好用，但书上更新慢，导致很少见
    {
        struct timespec time2 = {
            .tv_sec = 1,
            .tv_nsec = 200 * 1000 * 1000,
        };
        nanosleep(&time2, NULL);
    }
    // 注意用 200 * 1000 * 1000 这样的方式表示数字是一个技巧
    // gl 中可以很轻松定义一个新的数字字面量表示方法，比如这样 200_000_000
    // c 语言中只能用这种折中的办法
    // 如果不这样写，太容易出错了（注意到 18 行的数字多了一个 0 吗？）
    // 工具都是有局限性的，我们要自己改造，绕过去，解决问题


    // 方法 3
    // 直接传常量指针进去
    // 这个应该很罕见
    nanosleep((struct timespec[]){{1, 200000000L}}, NULL);
    // 其实还可以这样
    // nanosleep((struct timespec[]){{.tv_sec=1, .tv_nsec=200000000L}}, NULL);


    return 0;
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

            // 在 unix 中
            // socket 也被抽象为文件
            // 因此我们可以直接用 write 来写入数据
            //
        char *msg = "HTTP/1.1 200\r\nContent-Type: text/plain\r\nConnection: close\r\nContent-Length: 2\r\n\r\nok";
        write(clientSocket, msg, strlen(msg));

         // 我们也可以用 read 从 clientSocket 读取客户端发来的数据
        const int len = 800;
        char buffer[len+1];
        read(clientSocket, buffer, len);
        printf("msg: (%s)\n", buffer);

        // // 响应一个请求后sleep
         struct timespec time1;
        time1.tv_sec = 1;
        time1.tv_nsec = 200000000;
        // 通常会要求在数字的后面加上 L 来表示这是一个 long 类型
        // long 在 64 位系统中是 8 字节
        // 2000000000L
        nanosleep(&time1, NULL);

            // 关闭连接
        close(clientSocket);
    }    
    return 0;
}