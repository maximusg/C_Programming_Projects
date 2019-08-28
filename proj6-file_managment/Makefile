APPS = mycopy
all: $(APPS)

mycopy: mycopy.c 
	gcc -Wall mycopy.c -o mycopy
r:
	./mycopy hello.txt hello
clear:
	rm -f hello

dist: 
	tar -cvf proj6.tar Makefile mycopy.c

clean:
	rm -f mycopy
