#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        puts("This is the child process");
    } else {
        wait(NULL);  // Parent waits for child
        puts("This is the parent process");
    }

    return 0;
}
