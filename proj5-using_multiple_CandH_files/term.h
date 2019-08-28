// ----------------------------------------------------------------------
// file: term.h
//
// Description: This is the header file for the TERMINAL module of the 
//     blacjack program.
//
// Created: 2018-11-01 (P. Clark)
//
// Modified:
// ----------------------------------------------------------------------
#ifndef TERM_H
#define TERM_H

// initialize the terminal to our liking
extern int term_init(void);

// Get the user's reply to the game's prompt
extern int term_input(void);

#endif
// end of term.h
