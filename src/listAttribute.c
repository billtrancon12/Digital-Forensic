#define _LARGEFILE64_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include "utility.h"
#include "attrHeader.h"

const int SIZE_OF_ENTRY = 1024;
const int LIST_ONLY = 0;
const int LIST_AND_RECOVER = 1;


/**
 * If it is in LIST_ONLY mode, it will only list attributes
 * If it is in LIST_AND_RECOVER mode, it will attempt to create a new file to recover the deleted file
*/
uint8_t listAttributes(int fd, char* recovery_path, uint64_t mft_hex_address, uint64_t entry_number, uint32_t* types, int types_len, int mode){
    uint64_t entry_addr = entry_number * 0x400 + mft_hex_address;
    printf("The beginning address of entry number %ld is: 0x%lx\n", (long int)entry_number, (long int)entry_addr);
    
    uint8_t* buf = (uint8_t*)malloc(sizeof(uint8_t) * SIZE_OF_ENTRY);
    int lseekVal = lseek64(fd, entry_addr, SEEK_SET);

    if(lseekVal == -1){
        printf("Lseek failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
    }

    int readVal = read(fd, buf, SIZE_OF_ENTRY);
    if(readVal == -1){
		printf("Read failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if(readVal == 0){
		printf("Reached EOF! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}


    uint8_t offset_first_attr = buf[0x14];    // Get the offset to the first attribute 
	uint8_t file_type = buf[0x16];			  // To determine whether the file is in used or not, a file or a directory

	switch (file_type)
	{
	case 0x00:
		printf("The file is not in used!\n");
		break;
	case 0x01:
		printf("The file is in used!\n");
		break;
	case 0x02:
		printf("The directory is not in used!\n");
		break;
	case 0x03:
		printf("The directory is in used!\n");
		break;
	default:
		printf("Something wrong!\n");
		break;
	}

    uint32_t pointer = offset_first_attr;
    int countFF = 0;
	int fd_recover = 0;
    while(pointer < SIZE_OF_ENTRY && countFF < 4){
        if(buf[pointer] == 255){
            countFF++;
            continue;
        }
        countFF = 0;
		uint32_t attr_id = read4Byte(buf, SIZE_OF_ENTRY, pointer);
        uint32_t attr_len = read4Byte(buf, SIZE_OF_ENTRY, pointer + 4);
		
		for(int i = 0; i < types_len; i++){
			uint32_t type = types[i];
			if((type == 0x00 && attr_id == 0x30) || (attr_id == type && type == 0x30)){
	        	printf("Attribute ID is: %02X and Attribute Length is: %02X\n", attr_id, attr_len);
				fd_recover = listNameAttribute(recovery_path, buf + pointer, attr_len, mode);
				
				if(mode != LIST_ONLY && (fd_recover == EXIT_FAILURE || fd_recover == EXIT_SUCCESS)){
					char* fname = "recovery_file.txt";
					char* absolute_path = (char*) malloc(sizeof(char) * (strlen(recovery_path) + strlen(fname) + 2));
					strcpy(absolute_path, recovery_path);
					strcat(absolute_path, "/");
					strcat(absolute_path, fname);
					fd_recover = open(absolute_path, O_WRONLY | O_CREAT);
					if(fd_recover == errno){
						printf("%s in recovery open\n", strerror(errno));
						return EXIT_FAILURE;
					}
				}
			}
			else if((type == 0x00 && attr_id == 0x80) || (attr_id == type && type==0x80)){
	        	printf("Attribute ID is: %02X and Attribute Length is: %02X\n", attr_id, attr_len);
				if(mode != LIST_ONLY && fd_recover == 0){
					char* fname = "recovery_file.txt";
					char* absolute_path = (char*) malloc(sizeof(char) * (strlen(recovery_path) + strlen(fname) + 2));
					strcpy(absolute_path, recovery_path);
					strcat(absolute_path, "/");
					strcat(absolute_path, fname);
					fd_recover = open(absolute_path, O_WRONLY | O_CREAT);
					if(fd_recover == errno){
						printf("%s in recovery open\n", strerror(errno));
						return EXIT_FAILURE;
					}
				}
				listDataAttribute(fd, fd_recover, mft_hex_address, buf + pointer, attr_len, mode);
			}
			else if(type == 0x00){  // flag for printing all type
	        	printf("Attribute ID is: %02X and Attribute Length is: %02X\n", attr_id, attr_len);
			}
		}
        pointer += attr_len;
    }
	close(fd_recover);
	free(buf);
	return EXIT_SUCCESS;
}

uint8_t listAttribute(int fd, char* recovery_path, uint64_t mft_hex_address, uint64_t entry_number, uint32_t type, int mode){
    uint64_t entry_addr = entry_number * 0x400 + mft_hex_address;
    printf("The beginning address of entry number %ld is: 0x%lx\n", (long int)entry_number, (long int)entry_addr);
    
    uint8_t* buf = (uint8_t*)malloc(sizeof(uint8_t) * SIZE_OF_ENTRY);
    int lseekVal = lseek64(fd, entry_addr, SEEK_SET);

    if(lseekVal == -1){
        printf("Lseek failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
    }

    int readVal = read(fd, buf, SIZE_OF_ENTRY);
    if(readVal == -1){
		printf("Read failed! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if(readVal == 0){
		printf("Reached EOF! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}


    uint8_t offset_first_attr = buf[0x14];    // Get the offset to the first attribute 
	uint8_t file_type = buf[0x16];			  // To determine whether the file is in used or not, a file or a directory

	switch (file_type)
	{
	case 0x00:
		printf("The file is not in used!\n");
		break;
	case 0x01:
		printf("The file is in used!\n");
		break;
	case 0x02:
		printf("The directory is not in used!\n");
		break;
	case 0x03:
		printf("The directory is in used!\n");
		break;
	default:
		printf("Something wrong!\n");
		break;
	}

    uint32_t pointer = offset_first_attr;
    int countFF = 0;
	int fd_recover = 0;
    while(pointer < SIZE_OF_ENTRY && countFF < 4){
        if(buf[pointer] == 255){
            countFF++;
            continue;
        }
        countFF = 0;
		uint32_t attr_id = read4Byte(buf, SIZE_OF_ENTRY, pointer);
        uint32_t attr_len = read4Byte(buf, SIZE_OF_ENTRY, pointer + 4);
		
		if((type == 0x00 && attr_id == 0x30) || (attr_id == type && type == 0x30)){
	        printf("Attribute ID is: %02X and Attribute Length is: %02X\n", attr_id, attr_len);
			fd_recover = listNameAttribute(recovery_path, buf + pointer, attr_len, mode);
		}
		else if((type == 0x00 && attr_id == 0x80) || (attr_id == type && type==0x80)){
	        printf("Attribute ID is: %02X and Attribute Length is: %02X\n", attr_id, attr_len);
			listDataAttribute(fd, fd_recover, mft_hex_address, buf + pointer, attr_len, mode);
		}
		else if(type == 0x00){  // flag for printing all type
	        printf("Attribute ID is: %02X and Attribute Length is: %02X\n", attr_id, attr_len);
		}
		
        pointer += attr_len;
    }
	free(buf);
	return EXIT_SUCCESS;
}


uint16_t getAttributeOffset(uint8_t* buf, uint32_t attr_len){
	uint8_t* dummyBuf = buf;
	if(dummyBuf == NULL){
		printf("Empty buffer! %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	uint8_t resident_flag = read1Byte(dummyBuf, attr_len, 0x08);
	uint8_t name_flag = read1Byte(dummyBuf, attr_len, 0x09);

	// non-resident named attribute
	if(resident_flag == 0x01 && name_flag > 0x00){
		return dummyBuf[0x20];
	}
	// non-resident non-name attribute
	if(resident_flag == 0x01 && name_flag == 0x00){
		return dummyBuf[0x20];
	}
	// resident no-name attribute
	if(resident_flag == 0x00 && name_flag == 0x00){
		return dummyBuf[0x14];
	}

	printf("Something wrong!\n");
	return -1;
}

// List name attribute and attempt to recover a file
int listNameAttribute(char* recovery_path, uint8_t* buf, uint32_t attr_len, int mode){
	uint8_t* dummyBuf = buf;
	if(dummyBuf == NULL){
		printf("Empty buffer! %s \n", strerror(errno));
		return EXIT_FAILURE;
	}
	uint16_t offset = getAttributeOffset(dummyBuf, attr_len);
	if(offset == -1){
		printf("Something wrong with the offset to attribute!\n");
		return EXIT_FAILURE;
	}
	// Go to the attribute
	dummyBuf += offset;
	
	uint64_t parent_dir = read4Byte(dummyBuf, attr_len - offset, 0x00);
	printf("\tParent directory's sequence number is: %lu\n", parent_dir);

	uint8_t fname_len = read1Byte(dummyBuf, attr_len - offset, 0x40);
	char* fname = (char*)malloc(sizeof(char)*fname_len + 1);
	fname[fname_len] = '\0';

	for(int i = 0; i < fname_len; i++){
		uint16_t charFileName = read2Byte(dummyBuf, attr_len - offset, 0x42 + i*2);
		fname[i] = charFileName;
	}
	printf("\tThe file name is: %s\n", fname);

	if(mode == LIST_AND_RECOVER){
		char* absolute_path = (char*) malloc(sizeof(char) * (strlen(recovery_path) + strlen(fname) + 2));
		strcpy(absolute_path, recovery_path);
		strcat(absolute_path, "/");
		strcat(absolute_path, fname);
		int fd_recover = open(absolute_path, O_WRONLY | O_CREAT);
		free(fname);
		if(fd_recover == errno){
			printf("%s in recovery open\n", strerror(errno));
			return EXIT_FAILURE;
		}
		return fd_recover;
	}
	free(fname);
	return EXIT_SUCCESS;
}

int listDataAttribute(int fd, int fd_recover, uint64_t mft_hex_address, uint8_t* buf, uint32_t attr_len, int mode){
	uint8_t* dummyBuf = buf;
	if(dummyBuf == NULL){
		printf("Empty buffer! %s \n", strerror(errno));
		return EXIT_FAILURE;
	}
	uint16_t offset = getAttributeOffset(dummyBuf, attr_len);
	if(offset == -1){
		printf("Something wrong with the offset to attribute!\n");
		return EXIT_FAILURE;
	}
		
	printf("The data attribute's resident flag is: 0x%02X (%s)\n", dummyBuf[0x08], (dummyBuf[0x08] == 0x01) ? "Non-resident flag" : "Resident flag");
	printf("The data attribute's name flag is: 0x%02X (%s)\n", dummyBuf[0x09], (dummyBuf[0x09] == 0x00) ? "No-name flag" : "Name flag");
	uint64_t actualSize = 0;
	if(offset == 0x40){
		actualSize = read8Byte(dummyBuf, attr_len, 0x30);
		printf("The actual size of file: %lu Bytes\n", actualSize);
		posix_fallocate64(fd_recover, 0, actualSize);
	}
	// Go to the attribute where the data attribute does not have data run
	dummyBuf += offset;
	if(offset != 0x40){
		actualSize = read8Byte(dummyBuf, attr_len - offset, 0x30);
		printf("The actual size of file: %lu Bytes\n", actualSize);
		posix_fallocate64(fd_recover, 0, actualSize);
	}
	// The data attribute has data run
	else{
		uint8_t cluster_info = read1Byte(dummyBuf, attr_len - offset, 0x00);
		uint32_t pointer = 0;
		int32_t previous_cluster = 0;
		uint64_t total_cluster_size = 0;
		while(cluster_info != 0x00){
			uint8_t cluster_len_bytes = cluster_info % 16;  // number of bytes of cluster_len
			uint8_t cluster_loc_bytes = cluster_info >> 4;  // number of bytes of location of first cluster
			uint32_t cluster_len = readNByte(dummyBuf, attr_len - offset, pointer + 1, cluster_len_bytes);
			int32_t cluster_loc = readNByte(dummyBuf, attr_len - offset, pointer + 1 + cluster_len_bytes, cluster_loc_bytes);
			int8_t lastByte_loc = read1Byte(dummyBuf, attr_len, pointer + cluster_len_bytes + cluster_loc_bytes);

			// Negative number!
			int32_t negative_cluster_loc = 0;
			if((lastByte_loc & 0x80) == 0x80){
				for(int i = 0; i < 4 - cluster_loc_bytes; i++){
					negative_cluster_loc = negative_cluster_loc << 8;
					negative_cluster_loc += 0xff;
				}	
				for(int i = cluster_loc_bytes; i > 0; i--){
					negative_cluster_loc = negative_cluster_loc << 8;
				}
				negative_cluster_loc += cluster_loc;
				cluster_loc = negative_cluster_loc;
			}
			printf("Cluster info 0x%02X\n", cluster_info);
			printf("\t 0x%0X 0x%0X (cluster_address, cluster_len)\n", cluster_loc, cluster_len);
			total_cluster_size += cluster_len * 0x1000;
			previous_cluster = (previous_cluster >= 0) ? cluster_loc + previous_cluster : previous_cluster + negative_cluster_loc;
			printf("\t 0x%0X 0x%0X (cluster_address, cluster_offset)\n", cluster_loc, previous_cluster);
			if(mode != LIST_ONLY){
				recoverData(fd, fd_recover, mft_hex_address, previous_cluster, cluster_len, (total_cluster_size > actualSize) ? actualSize % 4096 : 0);
			}
			pointer += cluster_len_bytes + cluster_loc_bytes + 1;
			cluster_info = read1Byte(dummyBuf, attr_len - offset, pointer);
		}
	}
	return EXIT_SUCCESS;
}

int recoverData(int fd, int fd_recover, uint64_t mft_hex_address, int32_t start_cluster_offset, uint32_t num_clusters, uint32_t remainingBytes){
	int lseekVal =  lseek64(fd, start_cluster_offset * 0x1000 + mft_hex_address - 0x4000, SEEK_SET);
	if(lseekVal == errno){
		printf("%s recover data lseek failed\n", strerror(errno));
		return EXIT_FAILURE;
	}

	int indexCluster = 0;
	for(indexCluster; indexCluster < num_clusters; indexCluster++){
		uint8_t* buf = (uint8_t*) malloc(sizeof(uint8_t) * 4096);
		int readVal = read(fd, buf, 4096);
		if(readVal == errno){
			printf("%s recover data read data failed\n", strerror(errno));
			return EXIT_FAILURE;
		}
		int writeVal = 0;
		if(remainingBytes == 0 || indexCluster < num_clusters - 1)
			writeVal = write(fd_recover, buf, 4096);
		else
			writeVal = write(fd_recover, buf, remainingBytes);
		if(writeVal == errno){
			printf("%s recover data write failed\n", strerror(errno));
			return EXIT_FAILURE;
		}
	}
}