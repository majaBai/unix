/*
在上个作业基础上，增加追加重定向符号的支持
/tmp/write.axe >> /tmp/write.output

这有 2 个实现方案
1，打开文件后 lseek 到文件末尾（看 man 2 lseek）
2，以追加模式打开（看 man 2 open）
*/


#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

char *fail = "null";

char*
getFullPath(char *cmd){
    if(strlen(cmd) < 1) {
        return fail;
    }
    char *path_env = getenv("PATH");
    char path[1024];
    char *dir;
    if (path_env == NULL) {
        printf("Environment variable PATH not found.\n");
        return fail;
    }
    strncpy(path, path_env, sizeof(path));
    dir = strtok(path, ":");
    // char full_path[1024];
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

    return found? full_path : fail;
}
void
readFrom(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("readFrom open");
    }
    const int size = 100;
    char buffer[size];
    ssize_t n = read(fd, buffer, size);
    if(n == -1) {
        perror("read");
    }
    buffer[n] = 0;
    fprintf(stderr, "[结果]: %s, %zd字节\n", buffer, n);
    close(fd);
}
char *
cmdFromStdin() {
    int max_len = 10;
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
    printf("cmd: %s %zd字节\n", command, strlen(command));
    return command;
}
int main(int argc, char **argv, char **envp) {
    char *path = "out.txt";

    while(1) {
        char *command = cmdFromStdin();
        char *cmd_p = getFullPath(command);
        free(command);
        // cmd_p 其实也需要 free, 但是下面的execve还需要使用，额...
        // 没有报错，但不晓得有没有隐形bug...
        
        pid_t pid = fork();
        if (pid == 0) {
            printf("enter child %d\n", pid);
            close(1);
            int fd = open(path, O_WRONLY | O_APPEND);
            if (fd == -1) {
                perror("child open");
                return 1;
            }

            printf("\n");
            char *argv[] = {"",NULL};
            execve(cmd_p, argv, (char *const *)envp);
        }else{
            printf("enter father, childpid=%d\n", pid);
            //等待当前子程序结束
            int status;
            pid_t child_to_exit = wait(&status);
            fprintf(stderr, "wait childpid %d : status=%d\n", child_to_exit, status);

            // 然后再读文件内容
            // readFrom(path);
        }
    }
    return 0;
}