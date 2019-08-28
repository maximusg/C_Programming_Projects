// ---------------------------------------------------------------------
// file: table.h
//
// Description: This is the header file for the TABLE module.
//
// Created: 2016-05-03 (P. Clark)
//
// Modfications:
// ---------------------------------------------------------------------
#ifndef TABLE_H
#define TABLE_H

extern int table_init(void);
extern int table_get_input(void);
extern int table_reset(void);
extern void table_player_card(const unsigned char suit,
                              const unsigned char pattern);
extern void table_dealer_card(const unsigned char suit,
                              const unsigned char pattern);
extern void table_player_lost(void);
extern void table_player_won(void);
extern void table_player_draw(void);
extern void table_exit(void);

#endif 
// end of table.h
