/*
在上个作业基础上，增加追加重定向符号的支持
/tmp/write.axe >> /tmp/write.output

这有 2 个实现方案
1，打开文件后 lseek 到文件末尾（看 man 2 lseek）
2，以追加模式打开（看 man 2 open）

编译： cc 4.8.c ./lib/m_str.c  -Ilib  -o 4.8.o
运行：（写入当前目录下的out.txt, 路径一定是 ./XXX, 不能没有最前面的点 .）
date >>  ./out.txt
ls >> ./out.txt
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/*
编译时指定头文件目录： -I lib 或者 -Ilib
*/
#include "m_str.h"

// 查找指令完整路径，找不到默认在当前目录，直接返回指令本身
char*
getFullPath(char *cmd){
    char *path_env = getenv("PATH");
    char path[1024];
    char *dir;
    if (path_env == NULL) {
        printf("Environment variable PATH not found.\n");
        return cmd;
    }
    strncpy(path, path_env, sizeof(path));
    dir = strtok(path, ":");
    char *full_path=  malloc(sizeof(char) * 1024);
    int found = 0;
    while(dir != NULL) {
        char temp[1024];
        snprintf(temp, sizeof(temp), "%s/%s", dir, cmd);
        if(access(temp, X_OK) == 0) {
            strcpy(full_path, temp);
            found =1;
            break;
        }
        dir = strtok(NULL, ":");
    }

    if (found) {
        return full_path;
    } else {
        // 说明 cmd 是当前文件夹下程序，比如/tmp/read.axe
        // 这种需要变成 ./tmp/read.axe 才能被执行
        if(cmd[0] == '.') {
            return cmd;
        } else {
            char *point = ".";
            return str_cat(point, cmd);
        }
    }
}


char *
cmdFromStdin() {
    int max_len = 256;
    char *command = malloc(sizeof(char) * (max_len + 1));
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
    printf("收到cmd: %s %zd字节\n", command, strlen(command));
    return command;
}


void
exe1(char *cmd, char *const envp[]){
    // 指令是否带参数，比如 ls /
    char **cmdAndArgs;
    char *del = " ";
    if(str_find(cmd, del) > -1) {
        cmdAndArgs = str_split(cmd, del);
    } else {
        cmdAndArgs[0] = cmd;
        cmdAndArgs[1] = NULL;
    }

    // 指令完整路径
    char *full_path = getFullPath(cmdAndArgs[0]);

    // char *args[] = {cmd, "/", NULL}; 
    char *args[] = {cmdAndArgs[0], cmdAndArgs[1], NULL };
    fprintf(stderr, "path: %s   cmd: %s   args: %s\n",full_path, cmdAndArgs[0], cmdAndArgs[1]);
    execve(full_path, args, envp);
    perror("execve");
}
char*
hasdel(char* cmd) {
    char *res = "noDel";
    char *del1 = "|";
    char *del2 = ">>";
    char *del3 = ">";
    if(str_find(cmd, del1) > 0) {
        return del1;
    }

    if(str_find(cmd, del2) > 0){
        return del2;
    }

    if(str_find(cmd, del3) > 0){
        return del3;
    }
    return res;
}
void
exeWithDel(char* command, char* del, const char **envp) {
    /*
    目前能处理的带分隔符的指令包括：
    管道： date | /tmp/read.o
    重定向：date > /out.txt
    */
    char **cmd = str_split(command, del);
    // 去掉空格
    char *cmd1 = str_trim(cmd[0]);
    char *cmd2 = str_trim(cmd[1]);

    printf("cmd1: %s \ncmd2: %s\n", cmd1, cmd2);

    if(strcmp(del, "|") == 0) {
        printf("管道 |\n");
        char * pipe = "/tmp/axe.fifo";
        mkfifo(pipe, 0777);

        pid_t pid1 = fork();
        if(pid1 == 0) {
            close(1);
            int fd = open(pipe, O_WRONLY);
            char *argv[] = {"",NULL};
            exe1(cmd1, (char *const *)envp);
        } else {
            pid_t pid2 = fork();
            if(pid2 == 0) {
                close(0);
                int fd = open(pipe, O_RDONLY);
                char *argv[] = {"",NULL};
                exe1(cmd2, (char *const *)envp);
            }
        }
    } else if (strcmp(del, ">>") == 0) {
        printf("追加重定向>>: %s \n", cmd2);
        pid_t pid1 = fork();
        if(pid1 == 0) {
            close(1);
            int fd = open(cmd2, O_WRONLY | O_APPEND);
            char *argv[] = {"",NULL};
            exe1(cmd1, (char *const *)envp);
        }
    }else if (strcmp(del, ">") == 0) {
        printf("重定向> \n");
        pid_t pid1 = fork();
        if(pid1 == 0) {
            close(1);
            int fd = open(cmd2, O_WRONLY);
            char *argv[] = {"",NULL};
            exe1(cmd1, (char *const *)envp);
        }
    }
}
int main(int argc, char **argv, const char **envp) {

    char *end = "exit";
    char *noDel = "noDel";
    while(1) {
        char *command = cmdFromStdin();
        if(strcmp(command, end) == 0 || command[0] == '\0') {
            printf("程序结束");
            break;
        }
        char *del = hasdel(command);
        if(strcmp(del, noDel) == 0) {
            // 没有 delimeter 直接执行
            pid_t pid1 = fork();
            if(pid1 == 0) {
                exe1(str_trim(command), (char *const *)envp);
            }
        } else {
            exeWithDel(command, del, envp);
        }
    }
    return 0;
}