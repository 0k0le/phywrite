#
# phyread makefile

phyread: phyread.o
	gcc phyread.o -o phyread

phyread.o: phyread.c
	gcc phyread.c -Wall -Wextra -pedantic -O2 -c -o phyread.o

install:
	mv phyread /usr/bin/phyread

clean:
	rm -f phyread.o phyread
