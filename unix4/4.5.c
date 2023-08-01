/*
【程序和进程和 fork】
在我们讨论专业技术问题的时候
我们讲的程序是一个可执行的文件

一个可执行文件可以运行很多次，可以同时运行很多个
运行中的可执行文件叫做【进程】


前面的作业中我们用 execve 这个 sc(system call) 启动了一个程序
但是问题在于那样我们没办法实现一个 shell
因为 shell 程序需要启动程序但自己不能被替换

所以 unix 中做这件事的标准方法是
1，进程先把自己复制一份（复制出来的进程称为 子进程）
2，在子进程中调用 execve 来执行程序

这个描述听起来有点让人哭笑不得，但 unix 的确是只有这样的机制
复制进程的 sc 叫做 fork
下面是一个例子


#include <unistd.h>
#include <stdio.h>


int
main(int argc, const char **argv) {
    pid_t pid = fork();
    printf("这句话会出现 2 次 %d\n", pid);
    if (pid == 0) {
        printf("this is child process\n");
        execve("/bin/date", NULL, NULL);
        printf("这句话不会出现, 因为 execve 了\n");
    } else {
        printf("this is parent process, child pid is %d\n", pid);
    }

    return 0;
}

使用 fork 来让执行的程序不替换自己

*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include<pwd.h>
#include <grp.h>
#include<time.h>
#include<sys/types.h>
#include <errno.h>

#include <dirent.h>
#include <stdbool.h>

int
exe1(const char *cmd, char *const envp[]){
    char *path_env = getenv("PATH");
    char path[1024];
    char *dir;
    if (path_env == NULL) {
        printf("Environment variable PATH not found.\n");
        return -1;
    }
    // strtok 会改变原path, 必须将 path_env 复制到缓冲区，
    // 即strtok会改变path,而不是path_env
    strncpy(path, path_env, sizeof(path));
    dir = strtok(path, ":");
    while (dir != NULL) {
        char full_path[1024];
        // snprintf(buf, size, format, arg1, arg2,....)
        // 将可变参数 agr1, arg2... 按照format格式写入 buf, 限制了长度为 size, 避免buf溢出
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        // printf("尝试路径：%s\n", full_path);
        if(access(full_path, X_OK) == 0) {
            // 找到了文件且有执行权限
            printf("****找到指令 %s***：%s\n",cmd,full_path);
            execve(full_path, (char *const[]){ (char *)cmd, NULL}, envp);
            perror("execve");
            break;
        }
        dir = strtok(NULL, ":");
    }
    printf("****未找到指令**: %s\n", cmd);
    return -1;
}
int
lsl(){
    char *path = getcwd(NULL, 0);
    DIR *dir = opendir(path);
    
    while(true) {
        struct dirent *dirData = readdir(dir);
        if(dirData != NULL) {
            char *f_name = dirData->d_name;

            // 跳过根目录.和上级目录..
            char *r1 = ".";
            char *r2 = "..";
            if(strcmp(f_name, r1) == 0 | strcmp(f_name, r2) == 0){
                continue;
            }
            
            // 文件的完整路径 = path + '/' + 文件名
            char p[1024];
            snprintf(p, sizeof(p), "%s/%s", path, f_name);

            //读文件状态信息
            struct stat info;
            int res = stat(p, &info);
            if(res !=0){
                perror("failed");
            } else {
                // file mode 权限
                char *m_str = malloc(sizeof(char) * 11);
                m_str[0] = (S_ISDIR(info.st_mode)) ? 'd' : '-';
                m_str[1] = (info.st_mode & S_IRUSR) ? 'r' : '-';
                m_str[2] = (info.st_mode & S_IWUSR) ? 'w' : '-';
                m_str[3] = (info.st_mode & S_IXUSR) ? 'x' : '-';
                m_str[4] = (info.st_mode & S_IRGRP) ? 'r' : '-';
                m_str[5] = (info.st_mode & S_IWGRP) ? 'w' : '-';
                m_str[6] = (info.st_mode & S_IXGRP) ? 'x' : '-';
                m_str[7] = (info.st_mode & S_IROTH) ? 'r' : '-';
                m_str[8] = (info.st_mode & S_IWOTH) ? 'w' : '-';
                m_str[9] = (info.st_mode & S_IXOTH) ? 'x' : '-';
                m_str[10] = '\0';
                printf("%s\t", m_str);

                // number of links
                printf("%ld\t", info.st_nlink); 

                // owner name
                struct passwd *user;
                user = getpwuid(info.st_uid);
                printf("%s\t", user->pw_name);

                // group name
                struct group *gdata;
                gdata = getgrgid(info.st_gid);
                printf("%s\t", gdata->gr_name);

                // number of bytes in the file
                printf("%ld\t", info.st_size);

                // last modified time
                time_t m_time = info.st_mtime;
                // 转换为可读时间字符串
                char time_str[100];
                char d_time[20] = {""};
                strftime(time_str, 100, "%m月 %d %H:%M", localtime(&m_time));
                strncpy(d_time, time_str + 1, strlen(time_str) - 1);
                printf("%s\t", d_time);

                //文件名
                printf("%s\t\n", dirData->d_name);
            }
        } else {
            break;
        }
    }
    closedir(dir);
    return 0;
}

int
main(int argc, const char **argv,const char **envp) {
    int max_len = 10;
    char command[max_len + 1];
    printf("请输入指令（不超过10字节）\n");
    while(1){
        int i = 0;
        char c;
        while(i < max_len && read(0, &c, 1) == 1) {
            if(c == '\n'){
                break;
            }
            command[i] = c;
            i++;
        }

        command[i] = '\0';
        if(command[0] == '\0'){
            break;
        }
        printf("收到的指令：%s\n", command);
        if(strcmp(command, "lsl") == 0){
            lsl();
            continue;
        }
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            exe1((const char *)command, (char *const *)envp);
        } else {
            // 父进程
        }
    }
    return 0;
}