// ------------------------------------------------------
// File: shell.c
//
// Name: Max Geiszler
//
// Description: Mimics a command shell. When the program is ran it will act
// as a normal bash command terminal, and allow access to most all commands
// with exceptiont to commands that need to be ran from the computer, such as "cd".
//
// Syntax: No syntax required. Just compile for prompt.
//
// ------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



#define BUFFER 80
#define SUCCESS 0
#define TRUE 0
#define MAXARGS 10
#define ALRM 30
#define DELIM " "
#define PROMPT "prompt> "
#define EXIT1 "exit"
#define SEGFAULT "segfault"


void alarm_handler(int signal) {

        switch (signal) {
        case SIGALRM:
                fprintf(stdout, "\nThe session has expired...\nExiting...\n");
                fflush(stdout);

                exit(3);
                break;
        case SIGINT:
                fprintf(stdout, "\nGood try... I don't die that easily.\n");
                fprintf(stdout, "Enter \'exit\' at the prompt to terminate this shell\n");
                fflush(stdout);

                break;
        case SIGSEGV:
                fprintf(stderr, "\nA segmentation fault has been detected.\nExiting...\n");
                fflush(stderr);

                exit(2);
                break;

        }

}

//borrowed some stuff from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
int execute(char ** args) {
        int pid;
        int result;
        int status;
        if ((pid = fork()) < 0) {
                fprintf(stderr, "ERROR: forking child");
                exit(EXIT_FAILURE);
        } else if (pid == 0) { //child
                errno = 0;
                result = execvp( * args, args);
                if (result != 0) {
                        perror("ERROR");
                        exit(EXIT_FAILURE); //no error message displayed
                }
                return 0;

        } else { //parent
                pid = waitpid(pid, & status, 0);
                WIFEXITED(status);
                return pid;
        }

}

int main() {
        struct sigaction act;
        act.sa_handler = alarm_handler;
        errno = SUCCESS;
        int loop = 1;
        char buf[BUFFER];
        char * args[MAXARGS];
        int length;
        int * bomb = NULL;

        act.sa_flags = 0;
        sigemptyset( & act.sa_mask);
        sigaction(SIGALRM, & act, NULL);
        sigaction(SIGSEGV, & act, NULL);
        sigaction(SIGINT, & act, NULL);

        if (errno != SUCCESS) {
                perror("Signal throw issue");
                exit(EXIT_FAILURE);
        }

        //starts shell
        do {
                printf(PROMPT);

                alarm(ALRM);
                fgets(buf, BUFFER, stdin);
                length = strnlen(buf, BUFFER);
                buf[length - 1] = '\0'; //remove \n replace with null term

                if (strcmp(buf, EXIT1) == TRUE) {
                        loop = 0;
                        printf("Exiting...\n");
                } else if (strcmp(buf, SEGFAULT) == TRUE) {
                        * bomb = 42;
                } else if (strcmp(buf, "") != TRUE) {
                        args[0] = strtok(buf, DELIM);
                        for (int i = 1; i < MAXARGS; ++i) {
                                args[i] = strtok(NULL, DELIM);
                                if (args[i] == NULL) {
                                        break;
                                }
                        }
                        execute(args);
                }
        } while (loop == 1);
        fflush(stderr);
        fflush(stdout);
        return EXIT_SUCCESS;
}
