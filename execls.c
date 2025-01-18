#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void execls(){
    pid_t pid = fork();

    if(pid > 0){
        printf("Executing ls");
        wait(NULL);
    }
    else if(pid == 0){
        printf("child process running...\n");
        execlp("ls" , "ls" , "-l", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }
    else{
        printf("fork failed");
        exit(EXIT_FAILURE);
    }

}



int main(){
    printf("WELCOME TO MY COMMAND LINE\n");
    printf("DO YOU WANT TO EXECUTE LS(Y/N)?\n");
    char choice;
    scanf(" %c" , &choice);
    if (choice == 'Y' || choice == 'y'){
        execls();
    }
    else{
        exit(1);
    }

}