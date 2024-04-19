#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include "br.h"
#include "mbr.h"
#include "getMftAddr.h"
#include "getPartition.h"
#include "utility.h"

int main(int argc, char** argv){
    // Check for arguments
	if(argc != 4){
		printf("Usage: ./test /dev/sdx #partition_number #entry_number \nwhere #partition_number is the partition number from 1-4");
        printf("\nwhere #entry_number is the entry number of a file\nwhere x is your device id\n");
		return EXIT_FAILURE;
	}
	if(validateDevice(argv[1]) == 1){
		printf("Please input /dev/sdx as your argument whereas x is your device id! \n");
		return EXIT_FAILURE;
	}
	if(validateDeviceNumber(argv[2]) == 1){
		printf("Please enter the partition number from 1-4\n");
		return EXIT_FAILURE;	
	}

	// Check open files
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		printf("Open failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	int partition_number = (int)(argv[2][0]) - 48 - 1; // 0-indexed
	// uint32_t hex_addr = mbrObj->pt[partition_number].first_sector_lba * 512;
	uint64_t mbr_hex_addr = getPartAddr(fd, partition_number);
    printf("The partition NTFS starts at 0x%lx\n", (long int)(mbr_hex_addr));

	if(mbr_hex_addr == 0){
		printf("The partition does not exist!\n");
		return EXIT_FAILURE;
	}
	uint64_t mft_hex_addr = getMftAddr(fd, mbr_hex_addr);
    printf("The MFT starts at address 0x%lx\n", (long int)(mft_hex_addr));

	printf("\n");
    int entry_number = 0;
    sscanf(argv[3], "%d", &entry_number);
    listAttribute(fd, NULL, mft_hex_addr, entry_number, 0x80, LIST_ONLY);
	// Close the stream
	close(fd);
	return EXIT_SUCCESS;
}
