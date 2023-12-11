// client.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<stdbool.h>



int
main(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in serv_addr;
    {
        // ip 应该是一个 32 位的数字
        // 系统提供了一个函数帮我们从字符串转换
        // 不然写起来太痛苦了
        const char *ip = "127.0.0.1";
        const short port = 3000;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(port);
    }

    
    // 连接服务器
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // 读取服务器传回的数据
    const int len = 200;
    char buffer[len+1];
    // char buffer[len+1] = {0};
    read(sock, buffer, len);

    printf("msg: (%s)\n", buffer);

    
    // 往服务器端发数据
    int i = 1;
    while(i < 10) {
        char *msg = "gua \n";
        write(sock, msg, strlen(msg));
        // printf("发送 %d\n", i);
        i += 1;
    }

    // 关闭socket
    close(sock);
    return 0;
}
