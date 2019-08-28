// ----------------------------------------------------------------------
// file: main.c
//
// Description: This is the main entry point for a blackjack program.
//     It implements the user menu and detection of winners.
//
// Created: 2016-05-03 (P. Clark)
//
// Modified:
// 2016-05-06 (P. Clark)
//     Set the terminal into raw (non-canonical) mode so that users do
//     not need to press Carriage Return when using menu.
// 2016-06-24 (P. Clark)
//     Registered an exit handler so things get cleaned up properly.
// 2016-10-26  (P. Clark)
//     Added 'static' to internal functions.
// 2018-05-09 (P. Clark)
//     Transitioned to the bool type.
// 2018-11-01 (P. Clark)
//     Started using the TERM module to initalize the terminal.
// ----------------------------------------------------------------------
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include "common.h"
#include "table.h"
#include "card.h"
#include "term.h"


#define DRAW_SCORE 16
#define BEST_SCORE 21
#define LOW_ACE 1
#define HIGH_ACE 11


// new type for keeping score of current hand for player and dealer
struct score_t {
        unsigned char num_aces;
        unsigned char tot_other;
};
static struct score_t Player_score;
static struct score_t Dealer_score;



// -----------------------------------------------------------------------
static int best_score(struct score_t score)
{
        int tot;

        // First calc the lowest possible score
        tot = score.tot_other + score.num_aces;

        // Now see if we can improve the score by replacing one of the
        // aces with an '11'. We can obviously only do that once or we
        // automatically go over.
        if (score.num_aces > 0) {
                if ((tot - LOW_ACE + HIGH_ACE) <= BEST_SCORE) {
                        tot = tot - LOW_ACE + HIGH_ACE;
                }
        }

        return tot;
} // best_score()



// -----------------------------------------------------------------------
static unsigned char isover(struct score_t score)
{
        unsigned char result;

        if (best_score(score) > BEST_SCORE) {
                result = true;
        }  else {
                result = false;
        }

        return result;
} // isover()



// -----------------------------------------------------------------------
static void update_score(struct score_t *score, unsigned char pattern)
{
        if (pattern > ACE && pattern < JACK) {
                score->tot_other = score->tot_other + pattern;
        } else if (pattern == ACE) {
                score->num_aces = score->num_aces + 1;
        } else {
                // face card
                score->tot_other = score->tot_other + 10;
        }
} // update_score()



// -----------------------------------------------------------------------
static void reset_scores(void)
{
        Player_score.num_aces  = Dealer_score.num_aces  = 0;
        Player_score.tot_other = Dealer_score.tot_other = 0;
} // reset_score()



// -----------------------------------------------------------------------
static void deal_cards(void)
{
        unsigned int i;
        unsigned char suit;
        unsigned char pattern;

        // deal two cards each 
        for (i=0; i < 2; ++i) {
                card_get(&suit, &pattern);
                table_player_card(suit, pattern);
                update_score(&Player_score, pattern);

                card_get(&suit, &pattern);
                table_dealer_card(suit, pattern);
                update_score(&Dealer_score, pattern);
        }
} // deal_cards()



// -----------------------------------------------------------------------
static void do_menu(void) {
        char input = 'Z';
        bool done = false;
        unsigned char suit;
        unsigned char pattern;
        unsigned char hitting;
        unsigned char natural_win;

        // One while loop per game
        while (!done) {
                // set up for another game
                hitting = true;
                natural_win = false;
                table_reset();
                reset_scores();
                deal_cards();

                // See if the player wins automatically with 21
                if (best_score(Player_score) == BEST_SCORE) {
                        hitting = false;
                        natural_win = true;
                }

                // what does player want to do?
                while (hitting) {
                        input = table_get_input();
                        switch (input) {
                                case 'h':
                                case 'H':
                                        // player wants to hit
                                        card_get(&suit, &pattern);
                                        table_player_card(suit, pattern);
                                        update_score(&Player_score,pattern);
                                        if (isover(Player_score)) {
                                                table_player_lost();
                                                hitting = false;
                                        }
                                        break;
                                case 's':
                                case 'S':
                                        // player wants to stand
                                        hitting = false;
                                        break;
                                case 'q':
                                case 'Q':
                                        // quit the game
                                        table_exit();
                                        hitting = false;
                                        done = true;
                                        break;
                                default:
                                        // any other key, do nothing
                                        break;
                        }
                }

                if (done) {
                        // do nothing
                } else if (natural_win) {
                        // check for a draw
                       if (best_score(Dealer_score) == BEST_SCORE) {
                                table_player_draw();
                       }
                } else if (!isover(Player_score)) {
                        // dealer's turn to choose if player is not over
                        while (best_score(Dealer_score) <= DRAW_SCORE) {
                                // Dealer must take a hit
                                card_get(&suit, &pattern);
                                table_dealer_card(suit, pattern);
                                update_score(&Dealer_score,pattern);
                        }
                        if (isover(Dealer_score)) {
                                table_player_won();
                        } else if (best_score(Player_score) > 
                                   best_score(Dealer_score)) {
                                table_player_won();
                        } else if (best_score(Player_score) ==
                                   best_score(Dealer_score)) {
                                table_player_draw();
                        } else {
                                table_player_lost();
                        }
                }
                reset_scores();
        }
} // do_menu()



// *********************************************************************
// **************************** M A I N ********************************
// *********************************************************************
int main(void)
{
        int result = SUCCESS;

        // initialize the Card module
        card_init();

        // initialize the table
        result = table_init();
        if (result != SUCCESS) {
                printf("The board could not be initialized.\n");
        } else {
                // Initalize the terminal to our liking
                result = term_init();
        }
        
        if (result == SUCCESS) {
                // initialize score
                reset_scores();

                // start the game
                do_menu();
        }

        return result;
} // main()

// end of main.c
