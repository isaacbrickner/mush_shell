#include <stdio.h> // printf() fgets() perror()
#include <stdlib.h> // exit()
#include <string.h> // strtok() strcmp()
#include <unistd.h> // fork() execvp()
#include <sys/wait.h> // wait()

int main(void){

    while (1) {
        char s[2048];
        char cwd[1024];

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working directory: %s\n", cwd);
        } else {
            perror("getcwd() error");
            return 1;
        }

        /* print to console and get user input */
        printf("%s >> ", cwd);
        fflush(stdout);
        fgets(s, sizeof s, stdin);


        /* tokenize the string and print each line */
        /* prepare commandline and arguments */
        char *clArgs[2048];  
        int i = 0;
        char *token = strtok(s, " \t\n\r");

        while (token != NULL) {
            clArgs[i] = token;
            i++;
            token = strtok(NULL, " \t\n\r");
        }
        clArgs[i] = NULL; // make last index null

        /* check if the first arg is cd and then cd to 2nd arg 
            which is the new directory. else check if user wants to exit */
        if (strcmp(clArgs[0], "cd") == 0) {
            if (chdir(clArgs[1]) == -1) {
                perror("chdir");
        }
            chdir(clArgs[1]);
            printf("Changed to directory: %s\n", clArgs[1]);
            /* TODO: add functionality to cwd when "cding"*/
            continue;
        } else if (strcmp(clArgs[0], "exit") == 0) {
            exit(0);
        }


    /* fork a new child process*/
    pid_t pid = fork();

    if (pid == 0) {
        /* print command and execute */
        printf("command is: %s. \n", s);
        execvp(clArgs[0], clArgs);
        perror("exec");
        exit(1);  // child dies here

    } else {
        puts("Parent: waiting for child to complete");
        wait(NULL);
        puts("Parent: child has exited");
        }
    }
    return 0;
}