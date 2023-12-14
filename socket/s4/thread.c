// thread.c
/*
这个程序演示了【多线程】的用法

编译命令如下
注意参数 -pthread 很奇怪，但这是规定
cc thread.c -pthread

本程序运行多次，每次结果都会不一样，试试
*/
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>



/*
这个 run 函数是一个【子线程入口函数】

这个函数必须接受一个 void * 指针
必须返回一个 void * 指针
*/
void *
run(void *args) {
    printf("thread run args(%p)\n", args);

    return NULL;
}

void
thread1(void) {
        // tid 是存储【线程的 id】的变量
    pthread_t tid;

        // 创建一个子线程  并且立即执行
        //
        // 四个参数的含义分别如下
        // 1. tid 的地址
        // 2. 线程的参数指针（我们目前只需要传 NULL）
        // 3. 子线程入口（这里是 run 函数）
        // 4. 传递给子线程的参数（这里是 NULL）
    pthread_create(&tid, NULL, run, NULL);


    {
            // 这里运行了 30 次 print
            // 每次程序运行都不能保证这 30 个 print 和 run 函数里面的内容的执行顺序
        for (int i = 0; i < 30; ++i) {
            printf("join before %d\n", i);

        }
            // join 的作用是，等待子线程结束再返回
            // 如果注释掉这一句，子线程很可能执行不了
            // 可以试试
        pthread_join(tid, NULL);
            // 下面这一句因为是在 join 之后
            // 所以一定在子线程之后运行
        printf("join after\n");
    }
}


int
main(void) {
    thread1();

    return 0;
}
