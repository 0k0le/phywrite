#
# phyread makefile

phyread: phyread.o
	gcc phyread.o -o phyread

phyread.o: phyread.c
	gcc phyread.c -Wall -Wextra -pedantic -O2 -c -o phyread.o

clean:
	rm -f phyread.o phyread
