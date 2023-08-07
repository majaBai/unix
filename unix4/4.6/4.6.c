/*
该程序能执行 /bin/date | /tmp/read.axe 这种指令
核心：实现管道功能 |
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
getFullPath(const char *cmd){
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
exe1(const char *cmd, char *const envp[]){
    char *full_path = getFullPath(cmd);
    execve(full_path, (char *const[]){ (char *)cmd, NULL}, envp);
    perror("execve");
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
    printf("cmd: %s %zd字节\n", command, strlen(command));
    return command;
}

int
str_index(char* str, char *c){
    int i = 0;
    while(i < strlen(str)) {
        if(str[i] == c[0]) {
            return i;
        }
        i++;
    }
    return -1;
}

char*
str_sub(char* str, int start, int end){
    int len = end - start + 1;
    char *res = malloc(sizeof(char) * (len + 1));
    for(int i = 0; i < len; i++) {
        res[i] = str[start +  i];
    }
    res[len] = '\0';
    return res;
}

char**
str_split(char* str, char* del) {
    char** res = malloc(sizeof(char *) * 64); // 假设结果最多分割64个字符串
    int num = 0;
    while(strlen(str) > 0) {
        int idx = str_index(str, del);
        if(idx == -1) {
            res[num] = str;
            return res;
        } else if (idx == 0) {
            // 开头
            char *sb_s = str_sub(str, 1, strlen(str) - 1);
            res[num] = str;
            return res;
        } else if (idx == strlen(str) - 1) {
            // 结尾
            char *sb_s = str_sub(str, 0, strlen(str) - 1);
            res[num] = str;
            return res;
        } else {
            // 中间
            char *sb_s = str_sub(str, 0, idx -1);
            res[num] = sb_s;
            num +=1;
            str = str_sub(str, idx + 1, strlen(str) - 1);
        }
    }
    return res;
}
int main(int argc, char **argv, char **envp) {
    char * pipe = "/tmp/axe.fifo";
    mkfifo(pipe, 0777);

    char *del = "|";
    char *end = "exit";
    while(1) {
        char *command = cmdFromStdin();
        if(strcmp(command, end) == 0 || command[0] == '\0') {
            printf("程序结束");
            break;
        }
        char **cmd = str_split(command, del);
        // char *c1 = "/bin/date";
        // char *c2 = "/tmp/read.axe"; 
        // char *cmd[] = {c1, c2};
        // /bin/ls|/tmp/read.axe
        printf("cmd0: %s \ncmd1: %s\n", cmd[0], cmd[1]);
        pid_t pid1 = fork();
        if(pid1 == 0) {
            close(1);
            int fd = open(pipe, O_WRONLY);
            char *argv[] = {"",NULL};
            execve(cmd[0], argv, envp);
        } else {
            pid_t pid2 = fork();
            if(pid2 == 0) {
                close(0);
                int fd = open(pipe, O_RDONLY);
                char *argv[] = {"",NULL};
                execve(cmd[1], argv, envp);
            }
        }
    }
    return 0;
}