#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int
main(int argc, char **arg) {
    while(1) {
        printf("[PID] %d  sleep 5s\n", getpid());
        sleep(5);
    }
 return 0;
}