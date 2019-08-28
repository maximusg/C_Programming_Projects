APPS = shell
all: $(APPS)

shell: shell.c 
	gcc -Wall shell.c -o shell
r:
	./shell hello.txt hello
clear:
	rm -f hello

dist: 
	tar -cvf proj7.tar Makefile shell.c

clean:
	rm -f shell
