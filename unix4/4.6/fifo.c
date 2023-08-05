#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>


extern char **environ;
/*
int mkfifo(const char *pathname, mode_t mode);
pathname：指定要创建的命名管道的文件路径名
mode：指定创建的管道的访问权限（权限掩码）,通过3个八进制数字表示，
      第一个数字表示所有者的权限
      第二个数字表示所有者所属组的权限
      第三个数字表示其他用户的权限

    每个数字又可以使用以下数值对应的权限组合：
        0：没有权限（---）。
        1：执行权限（--x）。
        2：写入权限（-w-）。
        3：执行权限和写入权限（-wx）。
        4：读取权限（r--）。
        5：读取权限和执行权限（r-x）。
        6：读取权限和写入权限（rw-）。
        7：读取权限、写入权限和执行权限（rwx）
例如0644, 表示所有者有读写权限，而所有者所属组和其他用户只有读权限

****************
int fprintf(FILE *stream, const char *format, ...);
    stream：指向要写入数据的文件的指针
    format：一个格式化字符串，用于指定输出的格式
    ...：可变参数列表，用于填充格式化字符串中的占位符
fprintf函数的工作方式与printf函数类似，但它将输出的结果不是显示在控制台上，而是写入到指定的文件中


*/

/*
#####子进程：
close(1) 关闭了 stdout -> 将 fd 文件指针变为标准输出 ->
执行 date，将输出到 stdout，也就是输出（写入）fd 指向的文件中

#####父进程：
close(0) 关闭了 stdin -> 将 fd 文件指针变为标准输入 -> 
read.axe 会从 stdin 也就是 fd 指向的文件中读取（此时读取的内容正是子进程写入文件的内容）
*/
int
main(int argc, char **argv, char **envp) {
    printf("fifo:参数 %s, env：%s\n", argv[0], envp[0]);
    // 下面演示了用 fifo 实现管道
    // 虽然你能在文件系统中看到一个 /tmp/axe.fifo 文件
    // 但实际上它是没有内容的，只是一个虚拟文件
    // 不同的进程可以打开这个文件来读写，这样就方便地实现了管道的功能
    char * path = "/tmp/axe.fifo";
    // 0777 是一个 8 进制数字
    // 表示 777 权限
    // 我们之前会用 S_I 之类的宏，这里就简化写法了，那样写代码太烦了
    mkfifo(path, 0777);

    pid_t pid = fork();
    /*
    
    */
    if (pid == 0) {
        /*
         这个进程先关掉标准输出 1
         这样 open 打开的 fd 就是 1 了

         文件描述符，是每个进程独立的东西
         每个进程启动后都会默认打开 标准输入 标准输出 标准错误输出 这三个文件
         分别占用了文件描述符 0 1 2
         那么我们再打开一个文件返回的描述符应该是 3，这是系统保证的，总是使用最小的未被使用的描述符

         现在我们关闭了 1
         那么再打开一个文件就会是 1

         execve 会加载 /bin/date 来替换本进程
         所以 /bin/date 的 1 实际就是本进程的 fd
         这样 /bin/date 里面往 1 写就等于是往 fd 里面写

         这是个奇特的设计，但是已经广泛使用了
        */
        close(1); //关闭了标准输出
        int fd = open(path, O_WRONLY);
        // 这里 fprintf(stderr 是 c 的库函数，相当于 write(2，因为我们 close(1) 了所以往 2 里面写才能显示出来
        fprintf(stderr, "[child FD] %d\n", fd);

        char *argv[] = {
            "/bin/date",
            NULL
        };
        
        execve(argv[0], argv, environ);
    } else {
        // 需要先把 read.c 编译为 /tmp/read.axe 这里才能运行到

        // 这个进程先关掉标准输入 0
        // 这样 fd 就是 0 了
        // 这样 /tmp/read.axe 从 0 里面读就是从 fd 里面读
        // 和上面一样
        close(0);
        int fd = open(path, O_RDONLY); 
        printf("[parent FD] %d\n", fd);

//        char *argv[] = {
//            "/tmp/read.axe",
//            NULL
//        };
        // 上面的代码可以用下面的方式写
        // 总之一定要以 NULL 结尾，不然没办法知道这个数组有多长
        // NULL 就是数字 0
        char **argv = malloc(sizeof(char *) * 2);
        char *path = "/tmp/read.axe";
        argv[0] = path;
        argv[1] = NULL;

        execve(argv[0], argv, envp);
    }

    return 0;
}