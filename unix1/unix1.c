// unix 1

// 世界上目前有 2 类广泛使用的主流操作系统（不包括实时系统比如汽车 飞机 飞船 工控）
// 1，unix（包括苹果 苹果手机 安卓 linux bsd 等等等等）
// 2，windows

// unistd.h 是 unix 系统调用头文件, write 需要它
// 一定要注意, 这个头文件必须先于其他头文件被 include
// string.h 是 strlen 需要的
// 其他函数在哪个头文件里面, 自行查阅
// 在终端输入 man 2 write 来查看文档，里面包含了头文件等信息
// man 2 表示要查的是 系统调用
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/// stdio.h 里面的 printf 等函数是对这些系统调用的包装
/// 【系统调用】就是操作系统提供的最底层 API
/// 这个例子演示了不用 C 标准库来进行输出
void
unix1_write(void) {
    // const char * 是标准的定义字符串常量套路
    const char *msg = "axe@kuaibiancheng.com";
    size_t len = strlen(msg);
    // write 是把数据写入到文件的系统调用
    // 参数 1 表示写到【标准输出】，默认的标准输出就是运行程序的终端
    // 参数 2 3 就不用解释了
    write(1, msg, len);
}


void
unix2_open(void) {
const char *path = "axe.kbc";
    // open 是打开一个文件的系统调用
    // OCREAT |_ O_RDWR 表示 【创建】 和 【读写】
    // S_IRWXU 表示 Read Write eXecute 权限（S_I 是啥意思你就自己查吧）
    // 末尾的 U 表示这是 用户 的权限
    // unix 的文件权限有 3 类，用户 用户组 所有人
    // 因为 x 发音是 ex 所以 ex 这种发音的东西外国人缩写都叫 X
    // 所以 user experience 这种名词的缩写是 UX

    int fileId = open(path, O_CREAT | O_RDWR, S_IRWXU);
    // 如你所见，这些命名简直就是冷战时期的产物（也的确是那时候的产物）
    // 什么一致性啊 可理解啊 不冲突啊都别想了
    // 这就是 unix

    // open 就是操作系统给你打开一个文件，然后你可以对文件进行读写操作
    // 返回的是一个 int 数字
    // 我们可以用这个数字去操作那个被打开的文件（比如读取内容 写入内容）
    // 系统内部会根据这个数字去识别你想要读写的文件是什么
    // 系统知道你是哪个程序，所以你的数字不会和别人的数字冲突

    // win 把这个数字叫做 【句柄】，把这个类型叫做 HANDLE
    // 定义如下
    // HANDLE CreateFileA(
    // LPCSTR lpFileName,
    // DWORD dwDesiredAccess,
    // DWORD dwShareMode,
    // LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    // DWORD dwCreationDisposition,
    // DWORD dwFlagsAndAttributes,
    // HANDLE hTemplateFile
    // );
    // 文档如下
    // https://docs.microsoft.com/zh-cn/windows/win32/fileio/creating-and-opening-files

    // unix 把这个数字叫做 【文件描述符】
    // 就像是忍者的查克拉 法师的魔力值 战士的豆汁气
    // 都只是对同一个概念的不同名词罢了

    // system 是 stdlib.h 里的 C 语言标准库函数
    // 这里是为了输出一下让我们看看当前目录下我们刚创建的 axe.kbc 文件
    system("pwd");
    system("ls -l");

    // 往文件里面写入一个新字符串
    const char *content = "axe rocks";
    size_t len = strlen(content);
    write(fileId, content, len);

    // 用完后要关闭文件
    // 这样别人才能打开
    // 当然这个例子里面可以不 close
    // 因为程序马上就退出了，打开的文件自然也就关闭了
    close(fileId);

    // 用 cat 把文件内容输出看看
    system("cat *.kbc");
}

/* 文件大小必须在 2G 以下*/
int
file_size(char* filename){
    FILE *fp = fopen(filename,"r"); 
    if(!fp) { 
        printf("fopen error");
        return -1; 
    };
    fseek(fp,0L,SEEK_END); 
    int size = ftell(fp); 
    fclose(fp); 
    return size;
}

int
main(void) {
    // unix1_write();
    // unix2_open();

    char *p = "axe.kbc";
    int size = file_size(p);
    printf("file sieze: %d", size);
    // main 函数是程序的入口
    // 返回 0 表示程序正常结束
    // unix 就是用 0 表示正常
    return 0;
}

/*
作业 1
实现 copy1.c 程序，它的功能如下
1，生成一个长度为 1M 的随机文件 1.source
2，把这个 1.source 文件用 read 逐字节都出来并逐字节写入 1.target
3，read 还没学，所以群里讨论吧
*/