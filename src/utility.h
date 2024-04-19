#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>

extern const int LIST_ONLY;
extern const int LIST_AND_RECOVER;

uint8_t read1Byte(uint8_t* buf, int bufSize, int index);
uint16_t read2Byte(uint8_t* buf, int bufSize, int indexStart);
uint32_t read4Byte(uint8_t* buf, int bufSize, int indexStart);
uint64_t read8Byte(uint8_t* buf, int bufSize, int indexStart);
uint64_t readNByte(uint8_t* buf, int bufSize, int indexStart, int nBytes);

// Print all attributes if the parameter type is 0x00
uint8_t listAttributes(int fd, char* recovery_path, uint64_t mft_hex_address, uint64_t entry_number, uint32_t* types, int types_len, int mode);
uint8_t listAttribute(int fd, char* recovery_path, uint64_t mft_hex_address, uint64_t entry_number, uint32_t type, int mode);
uint16_t getAttributeOffset(uint8_t* buf, uint32_t attr_len);

int listNameAttribute(char* recovery_path, uint8_t* buf, uint32_t attr_len, int mode);
int listDataAttribute(int fd, int fd_recover, uint64_t mft_hex_address, uint8_t* buf, uint32_t attr_len, int mode); 
int recoverData(int fd, int fd_recover, uint64_t mft_hex_address, int32_t start_cluster_offset, uint32_t num_cluster, uint32_t remainingBytes);
