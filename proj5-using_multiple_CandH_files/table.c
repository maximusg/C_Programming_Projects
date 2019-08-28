// ---------------------------------------------------------------------
// file: table.c
//
// Description: This file implements the TABLE module. It manages the
//     board table for a blackjack game.
//
// Created: 2016-05-03 
//
// Modifications:
// 2016-06-20 (P. Clark)
//     Display updated stats *before* saying you won/lost.
//     Hide the dealer's second card until it is supposed to be shown.
//     Allow player to press 'q' to quit instead of 'c' to continue.
//     Improved the overall look and feel, most notably the use of "cards".
// 2016-10-26 (P. Clark)
//     Added 'static' to internal functions.
// 2017-05-09 (P. Clark)
//     Got rid of a warning that only showed up with some compiler options.
// 2017-10-30 (P. Clark)
//     Changed table_exit so it only clears screen if module was properly
//     initialized.
// 2018-05-09 (P. Clark)
//     Fixed a bug that sometimes occurred when a bad suit was passed 
//     in a card.
//     Transitioned to the bool type.
// 2018-10-29 (P. Clark)
//     Used EXIT_SUCCESS. Started to use the Score module.
//     Moved actual reading of user input to TERM module.
// ---------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "common.h"
#include "card.h"
#include "table.h"
#include "image.h"
#include "score.h"
#include "term.h"


#define TABLE_MIN_COLS 80
#define TABLE_MIN_ROWS 24
#define TABLE_INITIALIZED 4567
#define RETURN 10
#define CLEAR_SCREEN "\033[2J"
#define MESSAGE_WIDTH 25
#define MESSAGE_START_ROW 12
#define MESSAGE_START_COL 54
#define MENU_WIDTH 14
#define MENU_HEIGHT 7
#define MENU_ROW 4
#define MENU_COL 2
#define STATS_ROW 15
#define STATS_COL 2
#define STATS_WIDTH 14
#define STATS_HEIGHT 7

#define RESET  "\033[0m"
#define BLACK  "\033[42;30m"
#define NORMAL "\033[42;30m"
#define RED    "\033[42;31m"
#define HIDDEN "\033[42;32m"
#define BLUE   "\033[42;34m"
#define WHITE  "\033[42;37m"
#define RED_ON_WHITE   "\033[47;31m"
#define BLUE_ON_WHITE  "\033[47;34m"
#define BLACK_ON_WHITE "\033[47;30m"

#define MOVE_CURSOR    "\033[%d;%dH"
#define MOVE_TOP_LEFT  "\033[1;1H"
#define MOVE_FOR_INPUT "\033[23;1H"

#define DEALER_ROW 4
#define DEALER_COL 25
#define PLAYER_ROW 15
#define PLAYER_COL 25
#define CARD_SHIFT 3

// Shapes for displaying cards (in unicode)
#define SPADE       "\u2660" /* black spade */
#define CLUB        "\u2663" /* black club */
#define HEART       "\u2665" /* red heart */
#define DIAMOND     "\u2666" /* red diamond */
#define TOP_LEFT    "\u250c" /* top-left corner */
#define TOP_RIGHT   "\u2510" /* top-right corner */
#define EDGE_HORZ   "\u2500" /* horizontal edge */
#define EDGE_VERT   "\u2502" /* vertical edge */
#define BOT_LEFT    "\u2514" /* bottom-left corner */
#define BOT_RIGHT   "\u2518" /* bottom-right corner  */
#define LEFT_TEE    "\u251c" /* left-tee for box */
#define RIGHT_TEE   "\u2524" /* right-tee for box */
//#define CARD_BACK   "\u2318" /* back of card */
//#define CARD_BACK   "\u2591" /* back of card */
#define CARD_BACK   "\u2573" /* back of card */
#define KING_SHAPE  "\u2654" /* symbol for King */
#define QUEEN_SHAPE "\u2655" /* symbol for Queen */
#define JACK_SHAPE  "\u2659" /* symbol for Jack */


// **********************************************************************
// ***************** G L O B A L    V A R I A B L E S *******************
// **********************************************************************
static unsigned int Table_state = 0;
static unsigned int Table_rows;
static unsigned int Table_cols;
static unsigned char Num_cards_dealer;
static unsigned char Num_cards_player;
static unsigned char Second_suit;
static unsigned char Second_pattern;
static bool Hidden_shown;


// **********************************************************************
// ***************** I N T E R N A L    F U N C T I O N S ***************
// **********************************************************************


static void move_cursor(int row, int col)
{
        printf(MOVE_CURSOR, row, col);
}


static void display_message(const char line1[], const char line2[])
{
        int i;
        int num;
        int row = MESSAGE_START_ROW;

        // print the top of the message box
        printf(RED_ON_WHITE);
        move_cursor(row++, MESSAGE_START_COL);
        printf(TOP_LEFT);
        for (i=0; i<MESSAGE_WIDTH-2; ++i) {
                printf(EDGE_HORZ);
        }
        printf(TOP_RIGHT);

        // print the sides of the box above the message
        move_cursor(row++, MESSAGE_START_COL);
        printf(EDGE_VERT);
        num = ((MESSAGE_WIDTH-2-strlen(line1)) / 2) + 1;
        for (i=1; i<num; ++i) {
                printf(" ");
        }

        // print the sides and the first line of the message
        printf("%s", line1);
        num += strlen(line1);
        for (i=num; i<MESSAGE_WIDTH-1; ++i) {
                printf(" ");
        }
        printf(EDGE_VERT);
        
        // print the sides and a blank link under the first line
        move_cursor(row++, MESSAGE_START_COL);
        printf(EDGE_VERT);
        for (i=1; i<MESSAGE_WIDTH-1; ++i) {
                printf(" ");
        }
        printf(EDGE_VERT);

        // print the sides and the second line of the message
        move_cursor(row++, MESSAGE_START_COL);
        printf(EDGE_VERT);
        num = ((MESSAGE_WIDTH-2-strlen(line2)) / 2) + 1;
        for (i=1; i<num && i<MESSAGE_WIDTH-2; ++i) {
                printf(" ");
        }
        printf("%s", line2);
        num += strlen(line2);
        for (i=num; i<MESSAGE_WIDTH-1; ++i) {
                printf(" ");
        }
        printf(EDGE_VERT);
        
        // print the bottom of the message box
        move_cursor(row++, MESSAGE_START_COL);
        printf(BOT_LEFT);
        for (i=0; i<MESSAGE_WIDTH-2; ++i) {
                printf(EDGE_HORZ);
        }
        printf(BOT_RIGHT);
        fflush(stdout);
}



static void print_suit_symbol(const unsigned char suit)
{
        switch (suit) {
                case SPADES:
                        printf(SPADE);
                        break;
                case CLUBS:
                        printf(CLUB);
                        break;
                case HEARTS:
                        printf(HEART);
                        break;
                case DIAMONDS:
                        printf(DIAMOND);
                        break;
                default:
                        printf("?");
                        break;
        }
} // print_suit_symbol()



static void set_suit_color(const unsigned char suit)
{
        // Set the color scheme appropriate to the suit
        switch (suit) {
                case HEARTS:
                case DIAMONDS:
                        printf(RED_ON_WHITE);
                        break;
                case SPADES:
                case CLUBS:
                        printf(BLACK_ON_WHITE);
                        break;
                default:
                        printf(BLUE_ON_WHITE);
                        break;
        }
} // set_suit_color()



static void draw_box(const int row, const int col, 
                     const int width, const int height)
{
        int i, j;
        int new_row = row;

        move_cursor(new_row++, col);
        printf(WHITE);
        printf(TOP_LEFT);
        for (i=0; i<width-2; ++i) {
                printf(EDGE_HORZ);
        }
        printf(TOP_RIGHT);

        move_cursor(new_row++, col);
        printf(EDGE_VERT);
        for (i=0; i<width-2; ++i) {
                printf(" ");
        }
        printf(EDGE_VERT);

        move_cursor(new_row++, col);
        printf(LEFT_TEE);
        for (i=0; i<width-2; ++i) {
                printf(EDGE_HORZ);
        }
        printf(RIGHT_TEE);

        for (i=0; i<height-4; ++i) {
                move_cursor(new_row++, col);
                printf(EDGE_VERT);
                for (j=0; j<width-2; ++j) {
                        printf(" ");
                }
                printf(EDGE_VERT);
        }

        move_cursor(new_row, col);
        printf(BOT_LEFT);
        for (i=0; i<width-2; ++i) {
                printf(EDGE_HORZ);
        }
        printf(BOT_RIGHT);
        printf(HIDDEN);
        fflush(stdout);
} // draw_box()



static void draw_stats(void)
{
        int row = STATS_ROW;
        int col = STATS_COL;
        unsigned int wins = 0;
        unsigned int losses = 0;
        unsigned int draws = 0;

        // Get the stats
        score_get_stats(&wins, &losses, &draws);

        draw_box(row++, col++, STATS_WIDTH, STATS_HEIGHT);
        printf(WHITE);
        move_cursor(row, col);
        printf("   Stats");

        row += 2;
        move_cursor(row++, col);
        printf("Wins   = %3d", wins);

        move_cursor(row++, col);
        printf("Losses = %3d", losses);

        move_cursor(row++, col);
        printf("Draws  = %3d", draws);
        fflush(stdout);
} // draw_stats()



static void draw_menu(void)
{
        // menu
        draw_box(MENU_ROW, MENU_COL, MENU_WIDTH, MENU_HEIGHT);
        printf(WHITE);
        move_cursor(MENU_ROW+1, MENU_COL+5);
        printf("Menu");
        move_cursor(MENU_ROW+3, MENU_COL+2);
        printf("H = Hit");
        move_cursor(MENU_ROW+4, MENU_COL+2);
        printf("S = Stand");
        move_cursor(MENU_ROW+5, MENU_COL+2);
        printf("Q = Quit");
} // draw_menu()



// Display the back of the dealer's hidden card
static void show_back(void)
{
        int i, j;
        int row, col;

        row = DEALER_ROW;
        col = DEALER_COL + CARD_SHIFT;

        // set place and color
        move_cursor(row, col);
        printf(BLUE_ON_WHITE);

        // top of card
        printf(TOP_LEFT); 
        for (j=0; j<CARD_WIDTH-2; ++j) {
                printf(EDGE_HORZ);
        }
        printf(TOP_RIGHT);

        // Middle of back of card
        for (j=1; j<CARD_HEIGHT-1; ++j) {
                move_cursor(++row, col);
                printf(EDGE_VERT);
                for (i=1; i<CARD_WIDTH-1; ++i) {
                        printf(CARD_BACK);
                }
                printf(EDGE_VERT);
        }

        // Bottom of card
        move_cursor(++row, col);
        printf(BOT_LEFT);
        for (j=0; j<CARD_WIDTH-2; ++j) {
                printf(EDGE_HORZ);
        } printf(BOT_RIGHT);
        printf(HIDDEN);
        fflush(stdout);
} // show_back



static void show_card(
        const unsigned char suit,
        const unsigned char pattern,
        const unsigned char row,
        const unsigned char col)
{
        int i, j;
        char symbol[3];
        unsigned char code;
        unsigned char internal_pattern = pattern;

        // get the right color scheme for the card/suit
        set_suit_color(suit);

        // Get the pattern/face value set to print
        if (pattern > ACE && pattern < 10) {
                symbol[0] = '0' + pattern;
                symbol[1] = '\0';
        } else if (pattern == 10) {
                strcpy(symbol, "10");
        } else {
                switch (pattern) {
                        case JACK:
                                strcpy(symbol,"J");
                                break;
                        case QUEEN:
                                strcpy(symbol,"Q");
                                break;
                        case KING:
                                strcpy(symbol,"K");
                                break;
                        case ACE:
                                strcpy(symbol,"A");
                                break;
                        default:
                                strcpy(symbol,"X");
                                internal_pattern = 1;
                                break;
                }
        }

        // move to starting position and start displaying card
        for (i=0; i<CARD_HEIGHT; ++i) {
                move_cursor(row+i, col);
                for (j=0; j<CARD_WIDTH; ++j) {
                        code = Image[internal_pattern][i][j];
                        if (code == 0) {
                                printf(" ");
                        } else if (code == 1) {
                                print_suit_symbol(suit);
                        } else if (code == 2) {
                                printf(TOP_LEFT);
                        } else if (code == 3) {
                                printf(EDGE_HORZ);
                        } else if (code == 4) {
                                printf(TOP_RIGHT);
                        } else if (code == 5) {
                                printf(EDGE_VERT);
                        } else if (code == 6) {
                                printf(BOT_LEFT);
                        } else if (code == 7) {
                                printf(BOT_RIGHT);
                        } else if (code == 8) {
                                printf(KING_SHAPE);
                        } else if (code == 9) {
                                printf(QUEEN_SHAPE);
                        } else if (code == 10) {
                                printf(JACK_SHAPE);
                        } else {
                                printf("?");
                        }
                }
        }

        // Display value of card at top
        move_cursor(row+1, col+1);
        printf("%s", symbol);

        // Display value of card at bottom
        if (pattern == 10) {
                move_cursor(row+CARD_HEIGHT-2, col+CARD_WIDTH-3);
        } else {
                move_cursor(row+CARD_HEIGHT-2, col+CARD_WIDTH-2);
        }
        printf("%s", symbol);

        printf(HIDDEN);
        printf(MOVE_TOP_LEFT);
        fflush(stdout);
} // show_card()



static void draw_table(void) 
{
        // give us a green table
        printf(NORMAL);
        printf(CLEAR_SCREEN);

        // Title
        printf(MOVE_TOP_LEFT);
        printf(WHITE);
        printf("                                     B L A C K J A C K\n");


        // Whose cards are whose?
        move_cursor(DEALER_ROW+2, DEALER_COL-3);     printf("D");
        move_cursor(DEALER_ROW+3, DEALER_COL-3);     printf("E");
        move_cursor(DEALER_ROW+4, DEALER_COL-3);     printf("A");
        move_cursor(DEALER_ROW+5, DEALER_COL-3);     printf("L");
        move_cursor(DEALER_ROW+6, DEALER_COL-3);     printf("E");
        move_cursor(DEALER_ROW+7, DEALER_COL-3);     printf("R");

        move_cursor(PLAYER_ROW+2, PLAYER_COL-3);     printf("P");
        move_cursor(PLAYER_ROW+3, PLAYER_COL-3);     printf("L");
        move_cursor(PLAYER_ROW+4, PLAYER_COL-3);     printf("A");
        move_cursor(PLAYER_ROW+5, PLAYER_COL-3);     printf("Y");
        move_cursor(PLAYER_ROW+6, PLAYER_COL-3);     printf("E");
        move_cursor(PLAYER_ROW+7, PLAYER_COL-3);     printf("R");

        draw_menu();
        draw_stats();

        printf(HIDDEN);
        printf(MOVE_TOP_LEFT);
        fflush(stdout);
} // draw_table()



// **********************************************************************
// ***************** E X T E R N A L    F U N C T I O N S ***************
// **********************************************************************

extern int table_init(void)
{
        struct winsize term;
        int result = SUCCESS;

        // Determine the coordinates of the terminal
        ioctl(0, TIOCGWINSZ, &term);
        Table_rows = term.ws_row;
        Table_cols = term.ws_col;
        if (Table_rows < TABLE_MIN_ROWS || Table_cols < TABLE_MIN_COLS) {
                printf("The terminal must have a minimimum size of "
                       "%d x %d\n", TABLE_MIN_COLS, TABLE_MIN_ROWS);
                result = FAILURE;
        } 
        
        if (result == SUCCESS) {
                // All is well initialize the table
                Table_state = TABLE_INITIALIZED;
                Num_cards_dealer = 0;
                Num_cards_player = 0;
                Hidden_shown = 0;

                // register the clean-up function to run at program termination
                atexit(table_exit);
        }

        return result; 
} // table_init()



extern int table_get_input(void)
{
        char input = 0;

        // Hide and move cursor, and then get user input 
        printf(HIDDEN);
        printf(MOVE_TOP_LEFT);
        fflush(stdout);
        input = term_input();

        // Move cursor back out of the way
        printf(MOVE_TOP_LEFT);
        fflush(stdout);

        return input;
} // table_get_input()



extern int table_reset(void)
{
        int result = SUCCESS;

        if (Table_state != TABLE_INITIALIZED) {
                result = FAILURE;
        } else {
                Num_cards_dealer = 0;
                Num_cards_player = 0;
                Hidden_shown = false;

                draw_table();
        }

        return result;
} // table_reset()



extern void table_exit(void)
{
        // Only clear the screen and move cursor if the table was
        // initialized. Without this test, if the terminal is too small,
        // then the error will not be seen by the user.
        if (Table_state == TABLE_INITIALIZED) {
                printf(RESET);
                printf(CLEAR_SCREEN);
                printf(MOVE_TOP_LEFT);
        }
        fflush(stdout);
} // table_exit()




extern void table_player_card(const unsigned char suit,
                              const unsigned char pattern)
{
        int col;
       
        col  = PLAYER_COL + (Num_cards_player * CARD_SHIFT);
        Num_cards_player++;
        show_card(suit, pattern, PLAYER_ROW, col);
} // table_player_card()



extern void table_dealer_card(const unsigned char suit,
                              const unsigned char pattern)
{
        int col;

        col = DEALER_COL + (Num_cards_dealer * CARD_SHIFT);

        if (Num_cards_dealer == 1) {
                // If this is dealer's second card, then save it for later
                // display (because it will be hidden from player).
                Second_suit = suit;
                Second_pattern = pattern;
                show_back();
        } else if (Num_cards_dealer == 2) {
                // We are now dealing third card -- display 2nd card now
                // before showing the third card.
                show_card(Second_suit, Second_pattern, 
                          DEALER_ROW, DEALER_COL + CARD_SHIFT);
                Hidden_shown = true;

                show_card(suit, pattern, DEALER_ROW, col);
        } else {
                show_card(suit, pattern, DEALER_ROW, col);
        }
        ++Num_cards_dealer;
} // table_dealer_card()



extern void table_player_won(void)
{
        int input;

        // show the dealer's second card
        if (!Hidden_shown) {
                show_card(Second_suit, Second_pattern, 
                          DEALER_ROW, DEALER_COL + CARD_SHIFT);
                Hidden_shown = true;
        }

        display_message("YOU WON!", "enter C to continue");

        // update stats
        score_inc_wins();
        draw_stats();

        printf(HIDDEN);
        printf(MOVE_TOP_LEFT);
        fflush(stdout);

        // wait 
        do {
                input = table_get_input();
                if (input == 'q' || input == 'Q') {
                        exit(EXIT_SUCCESS);
                }
        } while (input != 'C' && input != 'c');

} // table_player_won()



extern void table_player_draw(void)
{
        int input;

        // show the dealer's second card
        if (!Hidden_shown) {
                show_card(Second_suit, Second_pattern, 
                          DEALER_ROW, DEALER_COL + CARD_SHIFT);
                Hidden_shown = true;
        }

        score_inc_draws();
        draw_stats();
        display_message("DRAW", "enter C to continue");

        // wait 
        do {
                input = table_get_input();
                if (input == 'q' || input == 'Q') {
                        exit(EXIT_SUCCESS);
                }
        } while (input != 'C' && input != 'c');
} // table_player_draw()



extern void table_player_lost(void)
{
        int input;

        // show the dealer's second card
        if (!Hidden_shown) {
                show_card(Second_suit, Second_pattern, 
                          DEALER_ROW, DEALER_COL + CARD_SHIFT);
                Hidden_shown = true;
        }

        display_message("YOU LOST", "enter C to continue");

        // update stats
        score_inc_losses();
        draw_stats();

        printf(HIDDEN);
        printf(MOVE_TOP_LEFT);

        // wait 
        do {
                input = table_get_input();
                if (input == 'q' || input == 'Q') {
                        exit(EXIT_SUCCESS);
                }
        } while (input != 'C' && input != 'c');

} // table_player_lost()



// end of table.c
