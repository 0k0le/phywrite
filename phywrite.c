/*
 * Matthew Todd Geiger
 *
 * phywrite.c
 *
 * This program was intended to help with inspecting the physical memory of
 * TI's AM335 processors
 */

// STDC Includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// System includes
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>

#define PAGE_SIZE 	4096
#define PAGE_MASK 	(PAGE_SIZE-1)

typedef unsigned int uint;

#define FATAL(x, ...) { \
	fprintf(stderr, x "\n", ##__VA_ARGS__); \
	fflush(stderr); fflush(stdout); \
	perror(""); \
	exit(EXIT_FAILURE); \
}

void print_binary(uint);

// Read out content of physical memory
void write_phy_mem(off_t target, uint value) {
	// Open /dev/mem
	int fd = open("/dev/mem", O_RDWR|O_SYNC);
	if(fd == -1)
		FATAL("Failed to open /dev/mem");

	puts("/dev/mem opened");
	fflush(stdout);
		
	// Create a memory map that is byte aligned to PAGE_SIZE
	void *map_base = mmap(0, PAGE_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, fd, target & ~PAGE_MASK);
	if(map_base == (void *)-1)
		FATAL("Failed to mmap");

	// When calculating the virtual address remember to re-add what
	// you subtracted when doing byte alignment
	void *virt_addr = (void*)((uint)map_base + (target & PAGE_MASK));
	
	*((uint*)virt_addr) = value;
	uint readback = *((uint*)virt_addr);

	puts("READBACK");
	print_binary(readback);	

	close(fd);
}

void print_binary(uint contents) {
	for(int i = 31; i >= 0; i--) {
		if((i+1)%8 == 0 && i != 31)
			printf(" ");

		printf("%d", (contents >> i) & 1);
	}

	puts("");
}


int main(int argc, char **argv) {
	if(argc < 3) {
		printf("USAGE: %s <PHYSICAL_MEMORY_HEX> <VALUE_HEX>\n", argv[0]);
		return 3;
	}

	unsigned char onlyprint = 0;

	if(argc > 3) {
		onlyprint = (unsigned char)atoi(argv[3]);
	}
		

	off_t target = strtol(argv[1], 0, 0);
	uint value = (uint)strtol(argv[2], 0, 0);

	printf("Writing to 0x%lX: ", target);
	print_binary(value);

	if(onlyprint) {
		puts("Exiting early");
		return EXIT_SUCCESS;
	}

	// Write to physical memory
	write_phy_mem(target, value);		

	return EXIT_SUCCESS;
}






