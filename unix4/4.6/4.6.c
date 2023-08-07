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

    return found? full_path : cmd;
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

char*
str_trim(char *str) {
    int len = strlen(str);
    char *res = malloc(sizeof(char) * (len + 1));
    // 找到开头的空白停止位置
    int i = 0;
    while( i < len) {
        if(str[i] != ' ') {
            break;
        }
        i++;
    }
    if(i == len) {
        // 说明全是空格
        char *emp = "";
        res = emp;
        return res;
    }
    // 找到结尾的空白停止位置
    int j = len - 1;
    while(j >= 0) {
        if(str[j] != ' '){
            break;
        }
        j--;
    }
   
    char *sub = str_sub(str, i, j);
    strcpy(res, sub);
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


/*
char *c1 = "/bin/date";
char *c2 = "/tmp/read.axe"; 
char *cmd[] = {c1, c2};

/bin/ls|/tmp/read.axe
*/
void
exe1(char *cmd, char *const envp[]){
    // 指令是否带参数，比如 ls /
    char **cmdAndArgs;
    char *del = " ";
    if(str_index(cmd, del) > -1) {
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
int main(int argc, char **argv, const char **envp) {
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
        // 去掉空格
        char *cmd1 = str_trim(cmd[0]);
        char *cmd2 = str_trim(cmd[1]);
        printf("cmd1: %s \ncmd2: %s\n", cmd1, cmd2);

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
    }
    return 0;
}