/*
 * Matthew Todd Geiger
 *
 * phymem.c
 *
 * This program was intended to help with inspecting the physical memory of
 * TI's AM335 processors
 */

// STDC Includes
#include <stdio.h>
#include <stdlib.h>

// System includes
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

#define PAGE_SIZE 	4096
#define PAGE_MASK 	(PAGE_SIZE-1)

#define unsigned int uint

#define FATAL(x, ...) { \
	fprintf(stderr, x "\n", ##__VA_ARGS__); \
	fflush(stderr); fflush(stdout); \
	exit(EXIT_FAILURE); \
}

// Read out content of physical memory
uint read_phy_mem(off_t target) {
	// Open /dev/mem
	int fd = open("/dev/mem", O_RDONLY|O_SYNC);
	if(fd == -1)
		FATAL("Failed to open /dev/mem");

	puts("/dev/mem opened");
	fflush(stdout);
		
	// Create a memory map that is byte aligned to PAGE_SIZE
	void *map_base = mmap(0, PAGE_SIZE, PROT_READ, MAP_SHARED, fd, target & ~PAGE_MASK);
	if(map_base == (void *)-1)
		FATAL("Failed to mmap");

	// When calculating the virtual address remember to re-add what
	// you subtracted when doing byte alignment
	void *virt_addr = (void*)((uint)map_base + (target & PAGE_MASK));
	
	// Read value from memory
	uint value = *((uint *)virt_addr);

	close(fd);
	return value;		
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
	if(argc != 2) {
		printf("USAGE: %s <PHYSICAL_MEMORY>\n", argv[0]);
		return 3;
	}

	// Read the contents of physical memory
	off_t target = strtol(argv[1], 0, 0);
	uint contents = read_phy_mem(target);

	puts("\n--- INSPECTION ---");
	printf("0x%lX: 0x%X\n", target, contents);
	print_binary(contents);

	return EXIT_SUCCESS;
}






