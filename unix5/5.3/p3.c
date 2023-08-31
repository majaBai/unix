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
    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
        /*
        returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or
        by returning from main().
        */
        printf("terminated normally %d \n", status);

    } else if (WIFSTOPPED(status)) {
        printf("stopped by delivery of a signal %d \n", status);
    }
    
    return 0;
}