// ----------------------------------------------------------------------
// file: test.c
//
// Description: This is a simple program to test the card.c module.
//
// Created: 2017-04-18 (P. Clark)
//
// Modifications:
// 2017-10-31 (P. Clark)
//     Added a call to card_init.
//     Added some analysis of the information returned by card_get.
// 2018-10-31 (P. Clark)
//     Instead of just asking for a 53rd card and stopping, added code
//     that asks for a whole 2nd deck. Separated the test code into its
//     own function so it can be called any number of times.
// ----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "card.h"
#include "common.h"

#define NUM_SUITS 4
#define CARDS_PER_SUIT 13
#define NOT_DEFINED -1

unsigned int Seen_suit[NUM_SUITS+1];
unsigned int Seen_pattern[CARDS_PER_SUIT+1];
unsigned int Num_bad_patterns;
unsigned int Num_bad_suits;
unsigned int Last_suit = NOT_DEFINED;
unsigned int Last_pattern = NOT_DEFINED;


void display_card(unsigned char suit, unsigned char pattern)
{
        // Print human-readable version of the pattern
        if (pattern == ACE) {
                printf("Ace   of ");
        } else if (pattern > ACE && pattern < JACK) {
                printf("%-5d of ", pattern);
        } else if (pattern == JACK) {
                printf("Jack  of ");
        } else if (pattern == QUEEN) {
                printf("Queen of ");
        } else if (pattern == KING) {
                printf("King  of ");
        } else {
                printf("INVALID pattern: %d, ", pattern);
                ++Num_bad_patterns;
        }

        // Print human-readable version of the suit
        switch (suit) {
                case CLUBS:
                        printf("Clubs\n");
                        break;
                case HEARTS:
                        printf("Hearts\n");
                        break;
                case SPADES:
                        printf("Spades\n");
                        break;
                case DIAMONDS:
                        printf("Diamonds\n");
                        break;
                default:
                        printf("INVALID suit: %d\n", suit);
                        ++Num_bad_suits;
                        break;
        }
}



void test(void)
{
        int i;
        unsigned char suit;
        unsigned char pattern;
        bool all_good = true;

        // initialize stat's
        for (i=1; i < NUM_SUITS+1; ++i) {
                Seen_suit[i] = 0;
        }
        for (i=1; i < CARDS_PER_SUIT+1; ++i) {
                Seen_pattern[i] = 0;
        }
        Num_bad_patterns = 0;
        Num_bad_suits = 0;

        // Call card_get 52 times to see what we get
        for (i=0; i < CARDS_PER_DECK; ++i) {
                // pick a card...any card
                card_get(&suit, &pattern);
                if ((i==0) && (Last_suit == NOT_DEFINED)) {
                        // remember the first card ever dealt
                        Last_suit = suit;
                        Last_pattern = pattern;
                } else if (i==0) {
                        // This is not the first deck
                        if ((Last_suit == suit) && (Last_pattern == pattern)) {
                                printf("The 1st card of this deck is the "
                                       "same as the first card in the "
                                       "last deck.\n");
                        }
                        Last_suit = suit;
                        Last_pattern = pattern;
                }

                // show the card
                display_card(suit, pattern);

                // Update stats
                Seen_suit[suit]++;
                Seen_pattern[pattern]++;
        }
        printf("--------------------------------\n");

        // Did we get a pattern out of the proper range?
        if (Num_bad_patterns > 0) {
                printf("-Bad: %d bad patterns returned\n", Num_bad_patterns);
        } else {
                printf("-Good: No bad patterns returned\n");
        }

        // Did we get a suit out of the proper range?
        if (Num_bad_suits > 0 ) {
                printf("-Bad: %d bad suits returned\n", Num_bad_suits);
        } else {
                printf("-Good: No bad suits returned\n");
        }

        // Did we 13 of each suit?
        for (i=1; i < NUM_SUITS+1; ++i) {
                if (Seen_suit[i] != CARDS_PER_SUIT) {
                        printf("-Bad: suit %d had %d cards instead of %d\n",
                                i, Seen_suit[i], CARDS_PER_SUIT);
                        all_good = false;
                }
        }
        if (all_good) {
                printf("-Good: %d of every suit seen\n", CARDS_PER_SUIT);
        }


}



int main(int argc, const char *argv[])
{

        // initialize the card module
        card_init();

        // Test the first deck
        test();

        // Test the reshuffle and dealing of another 52 cards
        printf("--------------------------------\n");
        test();

        return EXIT_SUCCESS;
}


