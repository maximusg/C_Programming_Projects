APPS=randpass
all: $(APPS)

randpass: randpass.c 
	gcc -Wall randpass.c -o randpass
r20:	
	./randpass 20
r5:	
	./randpass 5
r:
	./randpass 10

dist: 
	tar -cvf proj3.tar Makefile randpass.c

clean:
	rm -f randpass
