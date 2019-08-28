// ------------------------------------------------------
// File: dots.c
//
// Name: Max Geiszler
//
// Description: This program when ran will clear the screen then print a #
// specified in the 1st arugment of "*" to the screen in random locations. 
//
// Syntax:
// Run the command with an argument as a number >1 <1000
// and watch magic ensue!
// ------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

#define SUCCESS 0
#define VALID_ARGS 2
#define FIRST_ARG 1
#define FIRST_ARG_MIN 1
#define FIRST_ARG_MAX 1000
#define MOVE_CURSOR "\033[%d;%dH"
#define CLEAR_SCREEN "\033[2J"
#define FIRST_ROWCOL 1
#define SLEEP_SECS 1
#define COLOR_MAX 245
#define COLOR_MIN 10
#define TXT_COLOR_RGB "\033[38;2;%d;%d;%dm"
#define BLACK_BG_RGB "\033[48;2;0;0;0m"

void display_dots(int num_dots)
{
        //initialize all the variables
        int max_rows =0;
        int max_cols=0;
        struct winsize win;
        ioctl(0,TIOCGWINSZ,&win);
        max_rows = win.ws_row;
        max_cols = win.ws_col;
        srandom(time(NULL));
        printf(CLEAR_SCREEN);
        int  rand_num_y=0;      
        int  rand_num_x=0;
        int  rand_cl_r=0;
        int  rand_cl_g=0;
        int  rand_cl_b=0;
         for (int i =0; i<num_dots;i++)
        {
                //Generate all the random numbers needed
                rand_num_y=(int) random()%max_rows +FIRST_ROWCOL;
                rand_num_x=(int) random()%max_cols +FIRST_ROWCOL;
                rand_cl_r=(int) random()%COLOR_MAX+COLOR_MIN;
                rand_cl_g=(int) random()%COLOR_MAX+COLOR_MIN;
                rand_cl_b=(int) random()%COLOR_MAX+COLOR_MIN;

                printf(MOVE_CURSOR, rand_num_y, rand_num_x);
                sleep(SLEEP_SECS);
                printf(BLACK_BG_RGB);
                printf(TXT_COLOR_RGB,rand_cl_r,rand_cl_g,rand_cl_b);
                printf("*");
                fflush(stdout);//flush the printf buffer
        } 
        printf(MOVE_CURSOR, max_rows, FIRST_ROWCOL);
}

int main(int argc, char *argv[])
{
        //Check number of Arguments
        if( argc !=VALID_ARGS)
        {
                printf("Must have %d argument(s)\n",VALID_ARGS-1);
                return EXIT_FAILURE;
        }        
        errno=SUCCESS;
        char *ch=NULL;
        long num_dots=0;
        num_dots =strtol(argv[FIRST_ARG],&ch,10);
        // verify string to long function was success, also make sure no
        // other input is in ARG1 other than numbers 
        if((errno !=SUCCESS) || (*ch !='\0')){
                printf("error first argument must only be number\n");
                return EXIT_FAILURE;
        }
        //Check size of first argument is correct value
        if(num_dots<FIRST_ARG_MIN || num_dots >FIRST_ARG_MAX)
        {
                printf("error number must be >%d and <%d\n",FIRST_ARG_MIN,FIRST_ARG_MAX);
                return EXIT_FAILURE;
        }
        //Run the display dots method
        display_dots(num_dots);
        return SUCCESS;
}
