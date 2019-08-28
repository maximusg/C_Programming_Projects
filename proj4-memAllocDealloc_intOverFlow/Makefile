APPS = rollover
all: $(APPS)

rollover: rollover.c 
	gcc -Wall rollover.c -o rollover
r:
	./rollover

dist: 
	tar -cvf proj4.tar Makefile rollover.c

clean:
	rm -f rollover
