// ----------------------------------------------------------------------
// file: card.c
//
// Description: This file implements the CARD module. Its job is to 
//     create an interface for providing cards from a shuffled deck of
//     52 standing playing cards. Each call to card_get() will return
//     the top card in that shuffled deck. If all the cards get used, 
//     then the deck is invisibly (and unknowingly) reshuffled.
//
// Created: 2016-05-03 (P. Clark)
//
// 2017-10-30 (P. Clark)
//     Added card_init().
// 2018-05-09 (P. Clark)
//     Transitioned to the bool type.
// 2018-10-31 (P. Clark)
//     Renamed Deck_shuffled to Needs_shuffling. Added some error checking.
//     Used EXIT_FAILURE.
// ----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "card.h"
#include "common.h"

#define CARDS_PER_SUIT 13
#define ATTRIBUTES_PER_CARD 2 
#define SUIT_COL    0
#define PATTERN_COL 1
#define CARD_STATE_INIT 42
#define RESET  "\033[0m"



// **********************************************************************
// ***************** G L O B A L    V A R I A B L E S *******************
// **********************************************************************

// Has the module been initialized?
static unsigned char Card_state = 0;

// Have the cards been shuffled?
static bool Needs_shuffling;

//CurDeck
static int CurDeck = 0;

// The deck of cards:
// Cards[i][SUIT_COL]    = the suit of the ith card
// Cards[i][PATTERN_COL] = the pattern of the ith card
static unsigned char Cards[CARDS_PER_DECK][ATTRIBUTES_PER_CARD];



// **********************************************************************
// ***************** E X T E R N A L    F U N C T I O N S ***************
// **********************************************************************

// The following function must be called before card_get() is called.
extern void card_init(void)
{
        int curcard = 0;
        int cursuit = 0;
        // Make sure this function is only called once
        if (Card_state == CARD_STATE_INIT) {
                printf(RESET);
                printf("card_init called twice\n");
                exit(EXIT_FAILURE);
        }

        // initialize the random number generator
        srandom(time(NULL));

        // set up a brand new deck with no shuffling whatsover
        // TBD student code goes here
        //allocate memory for cards

        for(int i =0; i<CARDS_PER_DECK; i++){
                for (int j =0; j<ATTRIBUTES_PER_CARD; j++){
                        if (j==0){
                                Cards[i][j] = cursuit++%DIAMONDS+1;//takes curcard and post increments
                        }else{
                                Cards[i][j] = curcard++%KING+1;//takes cursuit and post increments
                        }
                }
        }


        // Set the starting state of the module 
        Card_state = CARD_STATE_INIT;
        Needs_shuffling = true;
}



// Get a card from the current deck.
//
// suit: This is interpreted as follows:
//     1 = Clubs
//     2 = Hearts
//     3 = Spades
//     4 = Diamonds
//
// pattern: This is interpreted as the 
//     1 = Ace
//     2..10 as expected
//     11 = Jack
//     12 = Queen
//     13 = King
extern void card_get(unsigned char *suit, unsigned char *pattern)
{
        int randCard = 0;
        int randSwap = 0;
        int tempCard[ATTRIBUTES_PER_CARD];
        // Verify pointers are good
        if ((suit == NULL) || (pattern == NULL)) {
                printf(RESET);
                printf("card_get was passed bad pointers\n");
                exit(EXIT_FAILURE);
        }

        // Make sure we are ready to run...
        if (Card_state != CARD_STATE_INIT) {
                printf(RESET);
                printf("card_get was called before card_init\n");
                exit(EXIT_FAILURE);
        }

        if (Needs_shuffling) {
                for (int i = 0; i<CARDS_PER_DECK; i++){
                        randCard = random()%CARDS_PER_DECK;
                        randSwap = random()%CARDS_PER_DECK;
                        tempCard[0] = Cards[randCard][0];
                        tempCard[1] = Cards[randCard][1];
                        Cards[randCard][0] = Cards[randSwap][0];
                        Cards[randCard][1] = Cards[randSwap][1];
                        Cards[randSwap][0] = tempCard[0];
                        Cards[randSwap][1] = tempCard[1];      
                        
                }
                Needs_shuffling = false;
        }
        *suit = Cards[CurDeck][0];
        *pattern = Cards[CurDeck++][1];
        
        if (CurDeck==CARDS_PER_DECK){ 
                Needs_shuffling=true;
                CurDeck=0;
        }

} // card_get()


// end of card.c
