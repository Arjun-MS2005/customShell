#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAXCOMMANDSIZE 100
#define MAXARGS 100
#define MAXCOMMANDS 10

/* 
    TODO :
    1. Add built in command
    2. Add command history
    3. Enable Pipes 
    4. Enable redirection
*/

void parseCommand(char *command, char *args[]) {
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null terminate the arguments
}

void splitMultipleCommands(char *command, char *commands[]) {
    int i = 0;
    char *token = strtok(command, ";");
    while (token != NULL) {
        commands[i++] = token;
        token = strtok(NULL, ";");
    }
    commands[i] = NULL;  // Null terminate the commands
}

void executecd(char *args[]) {
    char *home = getenv("HOME");
    if (home == NULL) {
        home = "/";
    }
    
    if (args[1] == NULL) {
        if (chdir(home) == -1) {
            perror("Directory does not exist\n");
        }
    }
    else {
        if (chdir(args[1]) == -1) {
            perror("Directory does not exist\n");
        }
    }
}




void executeCommand(char *args[] , char * history[] , int * storedcommands ) {
    if(strcmp(args[0],"cd") == 0){
        executecd(args);
    }
    else if(strcmp(args[0] , "dsphist")==0){
        displayhistory(history , storedcommands);
    }
    else if (strcmp(args[0] , "clearhist") == 0){
        clearhist(history , storedcommands);
    }
    else{
        pid_t pid = fork();
        if (pid > 0) {
            printf("Executing Command...\n");
            wait(NULL);  // Wait for the child process to complete
        } else if (pid == 0) {
            printf("Child process running...\n");
            execvp(args[0], args);  // Execute the command
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }
}

void storecommands(char *command, char *history[], int *storedcommands) {
    // If history is full, shift the commands to make space
    if (*storedcommands == MAXCOMMANDS) {
        free(history[0]); // Free the memory for the oldest command
        for (int i = 1; i < MAXCOMMANDS; i++) {
            history[i - 1] = history[i]; // Shift the commands down
        }
        *storedcommands = MAXCOMMANDS - 1; // Adjust the count after shifting
    }

    // Allocate memory for the new command and store it
    history[*storedcommands] = strdup(command);
    (*storedcommands)++; // Increment the number of stored commands
}

void displayhistory(char * history[] , int *storedcommands){
    for(int i = 0 ; i < *storedcommands ; i++){
        printf("%d.%s\n" , i+1, history[i]);
    }
}


void clearhist(char * history[] , int *storedcommands){
    for (int i = 0 ; i < *storedcommands ; i ++){
        if(history[i] != NULL){
            free(history[i]);
            history[i] = NULL;
        }
    }
    *storedcommands = 0;
    printf("history is cleared\n");
}

void trimWhitespace(char *str) {  // Function to trim leading and trailing whitespaces
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
    char *commands[MAXARGS];
    char *history[MAXCOMMANDS];
    int storedcommands = 0;

    printf("WELCOME TO MY COMMAND LINE\n");
    while (flag) {
        printf(">>(type 'exit' to quit): ");
        if (fgets(command, MAXCOMMANDSIZE, stdin) != NULL) {
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';  // Remove the newline character
            }
        } else {
            printf("Error reading input\n");
            continue;
        }

        trimWhitespace(command);  // Trim leading and trailing spaces

        if (strcmp(command, "exit") == 0) {
            for (int i = 0; i < storedcommands; i++) {
                free(history[i]);
            }       
            printf("Thank you for trying this out!\n");
            flag = 0;
        } else {
            splitMultipleCommands(command, commands);  // Split input into multiple commands
            int i = 0;
            while (commands[i] != NULL) {
                trimWhitespace(commands[i]);  // Trim each individual command
                parseCommand(commands[i], args);  // Parse command into args
                storecommands(commands[i] , history , &storedcommands); //store commands
                executeCommand(args , history , &storedcommands);  // Execute the command
                i++;
            }
        }
    }

    return 0;
}
