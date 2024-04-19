#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "mbr.h"
#include "getPartition.h"

int MAX_DEVICE_STRING_SIZE=8;
char DEVICE_TEMPLATE[] = "/dev/sdx";

uint64_t getPartAddr(int fd, int partition_number){
	struct disk_mbr* mbrObj = (struct disk_mbr*) malloc(sizeof(struct disk_mbr)); // Create a MBR object 
	int readVal = read(fd, mbrObj, 512);	// Only read 512 bytes since MBR only has 512 bytes
	
	if(readVal == -1){
		printf("Read failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if(readVal == 0){
		printf("Reached EOF! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	/* Calculate the starting hex address of the partition. Get the number that the partition starts with.
	*  For example, the first_sector_lba is 2048 meaning the partition starts at sector number 2048th. 
	*  To calculate the hex address, since every sector is 512 bytes by default, unless format the drive differently,
	*  the address of the partition is first_sector_lba * 512
	*/ 
	// int partition_number = (int)(argv[2][0]) - 48 - 1; // 0-indexed
	uint64_t hex_addr = mbrObj->pt[partition_number].first_sector_lba * 512;
	free(mbrObj);
	return hex_addr;
}

int validateDevice(char* arg){
	int size = 0;
	char* dummyChar = arg;
	char* templateDummy = DEVICE_TEMPLATE;
	while(*templateDummy != '\0' && *dummyChar != '\0'){
		if(size < MAX_DEVICE_STRING_SIZE - 1 && *templateDummy != *dummyChar) return 1;
		size++;
		dummyChar++;
		templateDummy++;
	}
	if(size != MAX_DEVICE_STRING_SIZE) return 1;
	return 0;
}

int validateDeviceNumber(char* number){
	int size = 0;
	char* dummyChar = number;

	while(*dummyChar != '\0'){
		// Only from 1-4
		if(size > 0 || *dummyChar < 49  || *dummyChar > 52) return 1;
		size++;
		dummyChar++;
	}
	return 0;
}

int validateEntryNumber(char* number){
	char* dummyChar = number;

	while(*dummyChar != '\0'){
		// Only from 0-9
		if(*dummyChar < 48  || *dummyChar > 57) return 1;
		dummyChar++;
	}
	return 0;
}