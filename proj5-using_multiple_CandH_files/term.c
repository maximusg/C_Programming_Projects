// ----------------------------------------------------------------------
// file: term.c
//
// Description: This is the TERMINAL module of the blacjack program.
//     It knows how to change the terminal to our liking, and how to
//     return it to its prior setting before the game terminates.
//
// Created: 2018-11-01 (P. Clark)
//
// Modified:
// ----------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "common.h"

#define BYTES_2_READ 1
#define LINEFEED 10



static struct termios Old_trm; // original terminal settings
static bool   Changed = false;    // were terminal settings changed?



// This should be called before exiting
// -----------------------------------------------------------------------
static void term_exit(void) {
        if (Changed) {
                // set terminal settings back to what they were before
                tcsetattr(STDIN_FILENO, TCSANOW, &Old_trm);
        }
} // term_exit()



extern int term_input(void)
{
        char input = 0;
        ssize_t count = 0;

        do {
                // get one character from the user
                count = read(STDIN_FILENO, &input, 1);
        } while ((input == LINEFEED) && (count >= 0));

        return input;
} // term_input()



// initialize the terminal to our liking
extern int term_init(void)
{
        int result = SUCCESS;
        struct termios new_trm;

        // Put terminal into raw mode.
        // Borrowed from www.lafn.org/~dave/linux/terminalIO.html
        errno = SUCCESS;
        tcgetattr(STDIN_FILENO, &Old_trm); // get current settings
        if (errno != SUCCESS) {
                result = errno;
                perror("Error: unable to get terminal info");
        } else {
                new_trm = Old_trm;        // save current terminal config
                new_trm.c_cc[VMIN] = BYTES_2_READ; // return after 1 byte read
                new_trm.c_cc[VTIME] = 0;  // wait for 1 byte
                new_trm.c_lflag &= ~(ECHO|ICANON|IEXTEN|ISIG);
                tcsetattr(STDIN_FILENO, TCSANOW, &new_trm); // change now
                if (errno != SUCCESS) {
                        result = errno;
                        perror("Error: Unable to config terminal");
                } else {
                        Changed = true;
                }
        }
        
        if (result == SUCCESS) {
                // register exit handler
                atexit(term_exit);
        }

        return result;
} // term_init()


// end of term.c
