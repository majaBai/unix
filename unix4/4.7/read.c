#include <unistd.h>
#include <stdio.h>


int
main(void) {
    const int size = 100;
    char buffer[size];
    ssize_t n = read(STDIN_FILENO, buffer, size);
    buffer[n] = 0;

    printf("[read]: \n");
    printf("{%zd} %s\n", n, buffer);

    return 0;
}