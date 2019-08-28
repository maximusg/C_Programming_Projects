// ------------------------------------------------------
// File: proj1.c
//
// Name: Max Geiszler
//
// Description: This program requires two arguments
// which it will reverse the caps on any input for the 
// first argument and print to screen. 
// The second argument must be at most two digits where
// >0 and <25 which it will print sequentially to screen.
//
// Syntax:
// Give two arguments within confines of above description
// and watch magic ensue!
// ------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

#define SUCCESS 0
#define VALID_ARGS 3
#define FIRST_ARG 1
#define SECOND_ARG 2
#define SECOND_ARG_DIGITS 2
#define SECOND_ARG_MIN 1
#define SECOND_ARG_MAX 24
#define ZERO 0

int main(int argc, char * argv[]) 
    {

        int base = 10;
        int len = strlen(argv[SECOND_ARG]);
        char reverseCapStr[len + 1];
        long int secondArgument = strtoll(argv[SECOND_ARG], NULL, base); //Convert second arg to long int

        //Verify correct number of arguments
        if (argc != VALID_ARGS) {
                printf("Must provide %d arguments\n", VALID_ARGS - 1);
                exit(EXIT_FAILURE);
        }
        //Verify 2nd argument is correct length
        if (len > SECOND_ARG_DIGITS) {
                printf("Must provide %d digits or less for 2nd Arg\n", SECOND_ARG_DIGITS);
                exit(EXIT_FAILURE);
        }
        //Verify 2nd argument is digit
        for (int i = ZERO; i < len; i++) {
                if (!isdigit(argv[SECOND_ARG][i])) {
                        printf("2nd Arg must only be digits, char %d not a digit\n", i + 1);
                        exit(EXIT_FAILURE);
                }
        }
        errno = ZERO;
        //check if issue converting
        if (errno != ZERO) {
                printf("errno %d could not convert 2nd arg\n", errno);
                exit(EXIT_FAILURE);
        }
        //Verify 2nd argument is >0 <25

        if ((secondArgument < SECOND_ARG_MIN || secondArgument > SECOND_ARG_MAX)) {
                printf("2nd Arg %ld must be >%d and <%d\n ", secondArgument, SECOND_ARG_MIN, SECOND_ARG_MAX);
                exit(EXIT_FAILURE);
        }

        //switch caps on first arg
        len = strlen(argv[FIRST_ARG]);

        for (int i = ZERO; i <= len; i++) {
                if (i != len) {
                        if (islower(argv[FIRST_ARG][i])) {
                                reverseCapStr[i] = toupper(argv[FIRST_ARG][i]);
                        } else {
                                reverseCapStr[i] = tolower(argv[FIRST_ARG][i]);
                        }
                } else {
                        reverseCapStr[i] = '\0';
                }
        }
        //Prints the Reverse cap output to screen
        printf("%s\n", reverseCapStr);
        //Prints second arg count up directly to screen.
        for (long int i = 1; i <= secondArgument; i++) {
                if (i == secondArgument) {
                        printf("%ld\n", i);
                } else {
                        printf("%ld, ", i);

                }
        }

        return EXIT_SUCCESS;

}
