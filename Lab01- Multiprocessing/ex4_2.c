#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char command[100];

    while (1) {
        printf("myshell> ");
        fgets(command, sizeof(command), stdin);

        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0)
            break;

        int pid = fork();
        if (pid == 0) {
            execlp(command, command, NULL);
            perror("exec failed");
            exit(1);
        } else {
            wait(NULL);  // Wait for child
        }
    }

    return 0;
}
