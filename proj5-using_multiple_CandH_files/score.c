// ------------------------------------------------------
// File: score.c
//
// Name: Max Geiszler
//
// Description: Module for keep track of scoring in the blackjack game and returning the
// results when get status is called.
//
// Syntax:
// No arugrments necessary for this program.
// ------------------------------------------------------

static unsigned int Wins=0;
static unsigned int Losses=0;
static unsigned int Draws=0;

extern void score_inc_wins(void)
{
    Wins +=1;
}

extern void score_inc_losses(void)
{
    Losses +=1;
}

extern void score_inc_draws(void)
{
    Draws +=1;
}

extern void score_get_stats(unsigned int *wins, unsigned int *losses, unsigned int *draws)
{
    *wins=Wins;
    *losses=Losses;
    *draws = Draws;
}