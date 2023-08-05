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
exe1(const char *cmd, char *out, char *const envp[]){
    
    char *path_env = getenv("PATH");
    char path[1024];
    char *dir;
    if (path_env == NULL) {
        printf("Environment variable PATH not found.\n");
        return -1;
    }
    strncpy(path, path_env, sizeof(path));
    dir = strtok(path, ":");
    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        if(access(full_path, X_OK) == 0) {
            
            execve(full_path, (char *const[]){ (char *)cmd, NULL}, envp);
            perror("execve");
            break;
        }
        dir = strtok(NULL, ":");
    }
    return -1;
}
int
main(int argc, const char **argv,const char **envp) {
    char *out = "./tmp/write.out";
    int max_len = 10;
    char command[max_len + 1];
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
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            close(1);
            int fd = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            exe1((const char *)command, out, (char *const *)envp);
            close(fd);
        } else {
            // 父进程
        }
    }
    return 0;
}