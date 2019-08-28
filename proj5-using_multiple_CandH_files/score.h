// ------------------------------------------------------
// File: score.h
//
// Name: Max Geiszler
//
// Description: This is the header file for keeping track of score, and returning the results
// when get_status is called.
//
// Syntax:
// No arugrments necessary for this program.
// ------------------------------------------------------
extern void score_inc_wins(void); // increment the # of wins for the player
extern void score_inc_losses(void); // increment the # of losses for the player
extern void score_inc_draws(void); // increment the # of draws for the player
extern void score_get_stats( // get the current count of wins/losses/draws
unsigned int *wins,
unsigned int *losses,
unsigned int *draws);