APPS=dots 
all: $(APPS)

dots: 	dots.c 
	gcc -Wall dots.c -o dots
r100:	
	./dots 100
r5:	
	./dots 5
r:
	./dots 10

dist: 
	tar -cvf proj2.tar Makefile dots.c 

clean:
	rm -f dots
