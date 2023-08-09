// fifo.c
//
// cc fifo.c -o /tmp/fifo.axe
// /tmp/fifo.axe

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

extern char **environ;

int main(void)
{
    
    char *path = "/tmp/axe.fifo";
    mkfifo(path, 0777);
    pid_t pid = fork();
    if (pid == 0)
    {
        close(1);
        int fd = open(path, O_WRONLY);
        fprintf(stderr, "[child FD] %d\n", fd);
        char *argv[] = {
            "/bin/date",
            NULL};
        execve(argv[0], argv, environ);
    }
    else
    {
        close(0);
        int fd = open(path, O_RDONLY);
        printf("[parent FD] %d\n", fd);
        char **argv = malloc(sizeof(char *) * 2);
        char *path = "/tmp/read.axe";
        argv[0] = path;
        argv[1] = NULL;
        execve(argv[0], argv, environ);
    }
    return 0;
}