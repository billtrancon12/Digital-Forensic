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
#include <math.h>
#include "mbr.h"
#include "getPartition.h"
#include "utility.h"

int main(int argc, char** argv){
    // Check for arguments
	if(argc != 3){
		printf("Usage: ./test /dev/sdx #partition_number \nwhere #partition_number is the partition number from 1-4");
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
	int fd= open(argv[1], O_RDONLY);
	if(fd == -1){
		printf("Open failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	int partition_number = (int)(argv[2][0]) - 48 - 1; // 0-indexed
	uint64_t mbr_hex_addr = getPartAddr(fd, partition_number);
    printf("The partition Linux starts at 0x%lx\n", (long int)(mbr_hex_addr));

	if(mbr_hex_addr == 0){
		printf("The partition doeas not exist!\n");
		return EXIT_FAILURE;
	}

    int lseekVal = lseek64(fd, mbr_hex_addr + 0x400, SEEK_SET);
    if(lseekVal == errno){
        printf("%s lseek failed\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    uint8_t* buf = (uint8_t*) malloc(sizeof(uint8_t) * 1024);
    // Read superblock
    int readVal = read(fd, buf, 1024); 
    if(readVal == errno){
        printf("%s read failed\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if(buf[0x38] != 0x53 && buf[0x39] != 0xEF){
        printf("It is not a Linux partition! The partition may be corrupted!\n");
        return EXIT_FAILURE;
    }
    
	// Determine block size of the partition
	uint32_t block_size = pow(2, 10 + buf[0x18]);
	uint32_t totalBlock = read4Byte(buf, 1024, 0x4);
	free(buf);
	buf = (uint8_t*) malloc(sizeof(uint8_t) * block_size);

	lseekVal = lseek64(fd, mbr_hex_addr, SEEK_SET);
	if(lseekVal == errno){
		printf("%s lseek second failed\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	for(uint32_t blockCount = 0; blockCount < totalBlock; blockCount++){
		readVal = read(fd, buf, block_size);
		if(readVal == errno){
			printf("%s\n read failed in loop\n", strerror(errno));
			return EXIT_FAILURE;
		}

		if((buf[0] == 0x49 && buf[1] == 0x49 && buf[2] == 0x2A && buf[3] == 0x00) || (buf[0] == 0x4D && buf[1] == 0x4D && buf[2] == 0x00 && buf[3] == 0x2A)){
			printf("The potential first block of TIFF file is %d at hex address 0x%0lx\n", blockCount, (mbr_hex_addr + blockCount * block_size));
		}
	}

	close(fd);
	return EXIT_SUCCESS;
}