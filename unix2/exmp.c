/*
 作业 2.1
 实现一个 ls.c 程序
 如果有 一个参数，就把参数当 path
 如果没参数，就用当前目录当 path
 输出 path 下所有的文件的文件名和文件类型（只要求标注是目录还是普通文件）



 作业 2.2
 实现一个 lsl.c 程序
 给上面的 ls 程序增加一个可选参数 -l
 只能下面这样使用（-l 必须在最后）
 ls -l
 ls / -l

 -l 参数的输出格式如下，看不懂英语就群里问
 file mode, number of links, owner name, group name, number of
 bytes in the file, last modified time, the pathname

 -rw-r--r--    1 gua  groupname       360 1590569121 a.txt
 drwxr-xr-x   14 gua  groupname       448 1590508972 axe

*/


#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include <errno.h>


void
demo1(void) {
    /*
     getcwd 用于获取当前的目录
     第一个参数是 NULL 的话就会返回一个需要 free 的 char *
     */
    char *path = getcwd(NULL, 0);
    size_t len = strlen(path);

    // 下面的 write 第一个参数和上次不一样
    // 这次用了 STDOUT_FILENO 这个名字
    // 这个名字是在 unistd.h （unix standard）里面定义的, 参见:
    // https://zh.wikipedia.org/zh-hans/Unistd.h#%E6%96%87%E4%BB%B6%E5%86%85%E5%AE%B9
    // 是标准用法（虽然没人这么用）
    // 每个程序运行后系统都会打开 3 个文件，分别是 in out error
    //    #define     STDIN_FILENO    0    /* standard input file descriptor */
    //    #define    STDOUT_FILENO    1    /* standard output file descriptor */
    //    #define    STDERR_FILENO    2    /* standard error file descriptor */
    write(STDOUT_FILENO, path, len);
    write(STDOUT_FILENO, "\n", 1);
    free(path);

    // mkdir 用于创建一个目录
    const char *dirName = "axeDir";
    int permission = S_IRWXU;
    mkdir(dirName, permission);
    /*
     这里介绍一下 unix 下系统调用的错误处理方式
     基本上所有系统调用都会返回 int 值来表示调用成功还是失败
     0 表示成功
     非 0 表示失败

     当失败的时候，错误号会被写入一个全局变量 errno 中（头文件 #include<errno.h> ）
     下面的程序会再次创建一个 axeDir 目录，这就会失败，因为已经创建过了
     errno 会是 17
     17 代表什么意思你可以在 errno.h 头文件中看，里面有注释
     17 代表的是文件已经存在

     当然你可能找不到 error.h 在哪里
     那么简单的办法是 cc -E a.c 来拿到展开后的 a.c 文件
     cc -E a.c | grep mkdir

     错误号和错误描述是一个体力活，c 标准库中有 perror 函数来输出错误，用法如下
     */
    int e = mkdir(dirName, permission);
    if (e != 0) {
        printf("mkrid error (%d) (%d)\n", e, errno);
        // 看输出你可以知道 perror 啥意思
        perror("[error]");
    }
}



#include <dirent.h>
#include <stdbool.h>

void
demo2(void) {
    // opendir 打开一个目录
    char *path = "/home/maja/unix_w/unix2";
    DIR *dir = opendir(path);
    printf("open dir %p\n", dir);

    // 打开的目录可以用 readdir 来读取
    // 每次 readdir 就会返回一个新的文件指针，就像是读文件一样
    int numberOfFiles = 0;
    while (true) {
        struct dirent *dirData = readdir(dir);
        if(dirData != NULL) {
            /*
            dirData->d_type == dirData.d_type 类似
            d_type：
            DT_DIR 目录
            DT_REG 常规文件
            */ 
            if (dirData->d_type == DT_DIR) {
                printf("dir: ");
            } else {
                printf("file: ");
            }
            numberOfFiles++;
            printf("name：(%s) \n", dirData->d_name);

            // 这里介绍一个新的调用  lstat
            // 它用于读取文件的信息，具体看下面注释，看不懂英语就查单词（但是别整句翻译）
            // 这里给出的是 linux 下的内容，mac 下是不太一样的
            // struct stat fileInfo;
            // lstat(dirData->d_name, &fileInfo);
            // printf("info：(%d) (%ld) \n", fileInfo.st_uid, fileInfo.st_atim.tv_sec);
            /*
             struct stat {
                 dev_t     st_dev;         / ID of device containing file /
                 ino_t     st_ino;         / Inode number /
                 mode_t    st_mode;        / File type and mode /
                 nlink_t   st_nlink;       / Number of hard links /
                 uid_t     st_uid;         / User ID of owner /
                 gid_t     st_gid;         / Group ID of owner /
                 dev_t     st_rdev;        / Device ID (if special file) /
                 off_t     st_size;        / Total size, in bytes /
                 blksize_t st_blksize;     / Block size for filesystem I/O /
                 blkcnt_t  st_blocks;      / Number of 512B blocks allocated /
                 struct timespec st_atim;  / Time of last access /
                 struct timespec st_mtim;  / Time of last modification /
                 struct timespec st_ctim;  / Time of last status change /
             };
             */
        } else {
            break;
        }
    }
    printf("files：%d\n", numberOfFiles);
}


int
main(int argc, const char **argv) {
    /*
     main 的两个参数
     int argc 表示参数的个数
        程序自己是第一个参数
     const char **argv 是具体的参数
        所以 *argv（argv[0]）里面存的是一个 char *
         也就是说 argv[0] 是第 1 个参数的字符串
         也就是说 argv[1] 是第 2 个参数的字符串
        这不需要理解，知道这么用就好了，慢慢会理解的
    */
    // demo1();
    demo2();

    return 0;
}