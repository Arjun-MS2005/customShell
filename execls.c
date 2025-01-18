#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAXCOMMANDSIZE 100
#define MAXARGS 100

void parseCommand(char *command, char *args[]) {
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void executeCommand(char *args[]) {
    pid_t pid = fork();
    if (pid > 0) {
        printf("Executing Command...\n");
        wait(NULL);
    } else if (pid == 0) {
        printf("Child process running...\n");
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}

void trimWhitespace(char *str) { // function to trim leading and trailing white spaces
    int start = 0, end = strlen(str) - 1;

    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }

    while (end >= start && (str[end] == ' ' || str[end] == '\t')) {
        end--;
    }

    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }
    str[end - start + 1] = '\0';
}

int main() {
    int flag = 1;
    char command[MAXCOMMANDSIZE];
    char *args[MAXARGS];

    printf("WELCOME TO MY COMMAND LINE\n");
    while (flag) {
        printf(">>(type 'exit' to quit): ");
        if (fgets(command, MAXCOMMANDSIZE, stdin) != NULL) {
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';
            }
        } else {
            printf("Error reading input\n");
            continue;
        }

        trimWhitespace(command); 

        if (strcmp(command, "exit") == 0) {
            printf("Thank you for trying this out!\n");
            flag = 0;
        } else {
            parseCommand(command, args);
            executeCommand(args);
        }
    }

    return 0;
}
