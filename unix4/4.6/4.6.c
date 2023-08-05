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
getFullPath(char cmd[11]){
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
    char full_path[1024];
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

int main(int argc, char **argv, char **envp) {
    char * path = "/tmp/axe.fifo";
    mkfifo(path, 0777);

    while(1) {
        int max_len = 10;
        char command[max_len + 1];
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
        char *cmd_p = getFullPath(command);
        
        pid_t pid = fork();
        if (pid == 0) {
            printf("enter child %d\n", pid);
            //改变 stdout
            close(1);
            int fd = open(path, O_WRONLY);
            if (fd == -1) {
                perror("child open");
                return 1;
            }

            char *argv[] = {"",NULL};
            execve(cmd_p, argv, (char *const *)envp);
        }else{
            printf("enter father. childpid=%d\n", pid);
            //直接打开管道读结果,不要改变stdin
            // close(0)
            int fd = open(path, O_RDONLY);
            if (fd == -1) {
                perror("father open");
                return 1;
            }
            const int size = 100;
            char buffer[size];
            ssize_t n = read(fd, buffer, size);
            if(n == -1) {
                perror("read");
                return 1;
            }
            buffer[n] = 0;
            printf("[read]: %zd字节 %s\n", n, buffer);

            //等待当前子程序结束
            int status;
            pid_t child_to_exit = wait(&status);
            printf("wait childpid %d : status=%d\n", child_to_exit, status);

            close(fd);
        }
    }
    return 0;
}