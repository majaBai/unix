#include <unistd.h>
#include <stdio.h>


int
main(int argc, char **argv, char **envp) {
    printf("read:参数 %s, env：%s \n", argv[0], envp[0]);
    const int size = 100;
    char buffer[size];
    ssize_t n = read(STDIN_FILENO, buffer, size);
    buffer[n] = '\0';

    printf("[read]: {%zd} (%s)\n", n, buffer);

    return 0;
}