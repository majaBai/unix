/*
一般而言，unix 有如下 7 种 进程间通信（ipc）方法
1， 管道
2， fifo
3， signal
4， socket（本例子）
5， 信号量
6， 共享内存
7， 消息队列


socket 是实现网络通信的 api
不同的计算机都可以用 socket 通信
本机的不同进程之间当然也可以用这个功能通信


比如 游戏/聊天软件 都会用这样的方式通信
游戏一般会有一个启动器，你在启动器中登录
启动器作为服务器，去启动游戏程序，并且给游戏程序发数据
这样你就不用在游戏程序中重复登录了


这里提供了 2 个简单的例子
先运行 server 启动服务器
再运行 client 来连接服务器
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>



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
        // 创建一个端口为 3000 的服务器程序
        // ip 可以认为是计算机的名字（一个 32 位数字）
        // 端口可以认为是程序的名字（一个 16 位数字）
        // 不同的程序用不同的端口，就可以互相独立
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
         // 你自己试试
        const int len = 200;
        char buffer[len+1];
        read(clientSocket, buffer, len);
        printf("msg from client: (%s)\n", buffer);
            // 关闭连接
        close(clientSocket);
    }

    return 0;
}