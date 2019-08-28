// ------------------------------------------------------
// File: rollover.c
//
// Name: Max Geiszler
//
// Description: This program will generate a random list of 1-100 unsigned
// integers, then add them and print them out into 4 columns. 1 Column will check for
// overflow as unsigned integer, the other will not, and anotehr column will not check
// but it will be added in a long long int. The over flow will be shown with a red "RESTRICTED"
// once detected.
//
// Syntax:
// No arugrments necessary for this program.
// ------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>



#define CHAR_RED   "\033[31m"
#define CHAR_RESET "\033[0m"
#define MIN_NUMB 1
#define MAX_NUMBS 100
#define VALID_ARGS 1
#define MAX_RANDOM 1234567891//Max number of random chars in our sting that will be changed



void display(unsigned int values[], unsigned int num_values);

int main(int argc, char ** argv) {

        errno = 0;
        unsigned int randomNumbs = 0;
        unsigned int * values = NULL;
        srandom(time(NULL));
        unsigned int rand_size = ((unsigned int) random() % MAX_NUMBS) + MIN_NUMB;
        if (argc > VALID_ARGS) {
                printf("no arguments necessary\n still running function\n");
        }

        values = malloc(rand_size * sizeof(int));
        if ((errno != 0) || (values == NULL)) {
                printf("error memory allocation failed\n");
                return EXIT_FAILURE;
        }
        for (int i = 0; i < rand_size; i++) {
                randomNumbs = (unsigned int) random() % MAX_RANDOM;
                values[i] = randomNumbs;
        }

        display(values, rand_size);

        free(values);
        //set values to NULL!
        values=NULL;

}

void display(unsigned int values[], unsigned int num_values) {

        //vars for adding into
        unsigned int a = 0; //for compare
        unsigned int b = 0; //for compare
        unsigned int intWroll = 0;
        unsigned int intWOroll = 0;
        unsigned long long longWOroll = 0;
        int flag = 0; //flag to print RESTRICTED

        int l = 15; //length of col
        char * l1 = "unsigned";
        char * l2 = "int";
        char * l2a = "long long";
        char * l3 = "Subtotal";
        char * l4 = "Random";
        char * l4a = "w/ Rollover";
        char * l4b = "w/o Rollover";
        char * l5 = "Number";
        char * l5a = "Detection";
        char * l6 = "-------------";

        //print title:
        printf("%*s %*s %*s %*s\n", l, " ", l, l1, l, l1, l, l1); //line 1
        printf("%*s %*s %*s %*s\n", l, " ", l, l2, l, l2, l, l2a); //line 2
        printf("%*s %*s %*s %*s\n", l, " ", l, l3, l, l3, l, l3); //line 3
        printf("%*s %*s %*s %*s\n", l, l4, l, l4a, l, l4b, l, l4b); //line 4
        printf("%*s %*s %*s %*s\n", l, l5, l, l5a, l, l5a, l, l5a); //line 5
        printf("%*s %*s %*s %*s\n", l, l6, l, l6, l, l6, l, l6); //line 5

        //print first line
        a = values[0];
        intWOroll = a;
        longWOroll = a;
        intWroll = a;
        printf("%*u%*u%*u%*llu\n", l, a, l, a, l, a, l, (long long unsigned) a);

        //print each line
        for (int i = 1; i < num_values; i++) {
                a = values[i - 1];
                b = values[i];
                intWOroll += b;
                longWOroll += (long long unsigned) b;
                if ((intWroll > 0) && (intWroll > (UINT_MAX - b))) {
                        flag = 1;
                } else {
                        intWroll += b;
                }
                if (flag != 1) {
                        printf("%*u%*u%*u%*llu\n", l, b, l, intWroll, l, intWOroll, l, longWOroll);
                } else {
                        printf("%*u", l, b);
                        printf(CHAR_RED);
                        printf("%*s", l, "REJECTED");
                        printf(CHAR_RESET);
                        printf("%*u%*llu\n", l, intWOroll, l, longWOroll); //line 1
                        flag = 0;
                }

        }
        //printf("UINTMAX: %u\n",UINT_MAX);
}
