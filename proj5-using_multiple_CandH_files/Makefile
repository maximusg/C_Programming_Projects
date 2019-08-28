# ------------------------------------------------------------------------
#  This is the Make file for the blackjack program
#
#  Created: 2016-05-03 (P. Clark)
#
#  Modifications:
#  2017-04-18 (P. Clark)
#      Added a test target as a way of testing card.c.
#  2017-10-30 (P. Clark)
#      Updated dependencies and options. Added TERM and SCORE modules.
# ------------------------------------------------------------------------


OBJECTS=main.o table.o card.o score.o term.o

CFLAGS=-Wall -c -Os
LDFLAGS=-o

all: blackjack


blackjack: $(OBJECTS)
	gcc $(OBJECTS) $(LDFLAGS) blackjack

test: test.o card.o
	gcc test.o card.o -o test

main.o: main.c table.h common.h card.h term.h
	gcc $(CFLAGS) main.c

table.o: table.c table.h common.h card.h image.h score.h term.h
	gcc $(CFLAGS) table.c

card.o: card.c card.h common.h
	gcc $(CFLAGS) card.c

score.o: score.c score.h
	gcc $(CFLAGS) score.c

term.o: term.c term.h common.h
	gcc $(CFLAGS) term.c

test.o: test.c card.h common.h
	gcc $(CFLAGS) test.c

dist: 
	tar -cvf proj5.tar Makefile main.c card.c table.c term.c score.c test.c card.h table.h term.h score.h common.h image.h

clean:
	rm -f $(OBJECTS) blackjack test test.o




