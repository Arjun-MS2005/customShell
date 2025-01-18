#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAXCOMMANDSIZE 100

void execls() {
    pid_t pid = fork();

    if (pid > 0) {
        printf("Executing ls\n");
        wait(NULL);
    } else if (pid == 0) {
        printf("Child process running...\n");
        execlp("ls", "ls", "-l", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int flag = 1;

    printf("WELCOME TO MY COMMAND LINE\n");
    while (flag) {
        printf(">>(type 'exit' to quit): ");
        char command[MAXCOMMANDSIZE];
        if (fgets(command, MAXCOMMANDSIZE, stdin) != NULL) {
            size_t len = strlen(command);
            // Remove the newline character if present
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';
            }
        } else {
            printf("Error reading input\n");
            continue;
        }

        if (strcmp(command, "ls") == 0) {
            execls();
        } else if (strcmp(command, "exit") == 0) {
            printf("Thank you for trying this out!\n");
            flag = 0;
        } else {
            printf("Command '%s' is not supported.\n", command);
        }
    }

    return 0;
}
