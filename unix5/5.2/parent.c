#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>


int
main(int argc, char **arg, const char **envp) {
    int pid = fork();
    if(pid == 0) {
        // 子进程
        char *const args[] = {"", NULL};
        execve("./child.o",args, (char *const *)envp);
    }
    int status;
    wait(&status);
    printf("wait %d \n", status);
 return 0;
}