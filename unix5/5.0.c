/*
作业内容：补全 add 函数, 使用作业通过测试
本次作业只能在 linux 中完成

# 本作业要先装库，命令如下
sudo apt-get install gcc-multilib clang

# 本作业需要要用如下命令编译 32 位二进制
# 32 位默认使用压栈传参（64 位是用寄存器传参，很麻烦，本次作业不涉及）
clang a.c -target i386
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

void
ensure(bool result, const char *msg) {
    if (!result) {
        printf("测试失败: <%s>\n", msg);
        exit(1);
    }
}

/*
下面的 add 函数是一个【可变参数列表】的函数
... 表示后面可以有 0 个或者多个参数

这个例子里，第一个参数是 argc，用于后续有 argc 个 int 类型参数
add 函数要把后续 argc 个 int 参数值累加并返回

后面的参数怎么获取呢？
参数是贴在一起存在函数调用栈中
所以用第一个参数的地址就可以计算出后续的参数的地址（加或减自己尝试）

又因为我们在这里约定了参数都是 int 类型，所以方便计算出其他参数的地址
*/
int
add(int argc, ...) {
    // printf("参数开始的地址：%p \n", &argc);
    int sum = 0;
    // &argc 是第一个参数的地址，在此基础行加减获取第二个参数的地址
    int *p = &argc;
    for(int i = 0; i < argc; i++) {
        p += 1; // int 类型指针，1 相当于实际内存地址 4
        sum += *p;
    }
    printf("sum %d \n", sum);
    return sum;
}
int
add1(int argc, ...) {
    int result = 0;
    va_list args;  // 定义一个 va_list 类型的变量
    va_start(args, argc);  // 初始化可变参数列表

    // 遍历所有可变参数并累加
    for (int i = 0; i < argc; i++) {
        int value = va_arg(args, int);  // 获取下一个 int 类型参数
        // printf("value** %d \n", value);
        result += value;
    }

    va_end(args);  // 结束可变参数的处理
    printf("result** %d \n", result);
    return result;
}

int
main() {
    int r1 = add(3, 1, 2, 3);
    ensure(r1 == 6, "测试 1");

    int r2 = add(2, 10, 5);
    ensure(r2 == 15, "测试 2");

    int r3 = add(7, 1, 2, 3, 4, 5, 6, 7);
    ensure(r3 == 28, "测试 3");

    return 0;
}

// 运行程序指令：clang 5.0.c -target i386 && ./a.out