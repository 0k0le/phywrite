#
# phywrite makefile

phywrite: phywrite.o
	gcc phywrite.o -o phywrite

phywrite.o: phywrite.c
	gcc phywrite.c -Wall -Wextra -pedantic -O2 -c -o phywrite.o

install:
	cp phywrite /usr/bin/phywrite

clean:
	rm -f phywrite.o phywrite
