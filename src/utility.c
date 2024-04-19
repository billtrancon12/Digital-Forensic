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

uint8_t read1Byte(uint8_t* buf, int bufSize, int indexStart){
    if(indexStart >= bufSize) return 0;
    uint8_t hex = 0;
    char* hex_str = (char*)malloc(sizeof(char)*3);
    hex_str[2] = '\0';
    sprintf(hex_str, "%02X", buf[indexStart]);
    hex = strtol(hex_str, NULL, 16);
    free(hex_str);
    return hex;
}

uint16_t read2Byte(uint8_t* buf, int bufSize, int indexStart){
    if(indexStart >= bufSize) return 0;
    uint16_t hex = 0;
    char* hex_str = (char*)malloc(sizeof(char)*5);
    hex_str[4] = '\0';
    for(int i = 0; i < 2; i++){
        if(indexStart + 1 - i >= bufSize) return 0;
        char* temp = (char*)malloc(sizeof(char)*3);
        sprintf(temp, "%02X", buf[indexStart + 1 - i]);
        hex_str[i*2] = temp[0];
        hex_str[i*2 + 1] = temp[1];
        free(temp);
    }
    hex = strtol(hex_str, NULL, 16);
    free(hex_str);
    return hex;
}

uint32_t read4Byte(uint8_t* buf, int bufSize, int indexStart){
    if(indexStart >= bufSize) return 0;
    uint32_t hex = 0;
    char* hex_str = (char*)malloc(sizeof(char)*9);
    hex_str[8] = '\0';
    for(int i = 0; i < 4; i++){
        if(indexStart + 3 - i >= bufSize) return 0;
        char* temp = (char*)malloc(sizeof(char)*3);
        sprintf(temp, "%02X", buf[indexStart + 3 - i]);
        hex_str[i*2] = temp[0];
        hex_str[i*2 + 1] = temp[1];
        free(temp);
    }
    hex = strtol(hex_str, NULL, 16);
    free(hex_str);
    return hex;
}

uint64_t read8Byte(uint8_t* buf, int bufSize, int indexStart){
    if(indexStart >= bufSize) return 0;
    uint64_t hex = 0;
    char* hex_str = (char*)malloc(sizeof(char)*17);
    hex_str[16] = '\0';
    for(int i = 0; i < 8; i++){
        if(indexStart + 7 - i >= bufSize) return 0;
        char* temp = (char*)malloc(sizeof(char)*3);
        sprintf(temp, "%02X", buf[indexStart + 7 - i]);
        hex_str[i*2] = temp[0];
        hex_str[i*2 + 1] = temp[1];
        free(temp);
    }
    hex = strtol(hex_str, NULL, 16);
    free(hex_str);
    return hex;
}

uint64_t readNByte(uint8_t* buf, int bufSize, int indexStart, int nBytes){
    if(nBytes == 1) return read1Byte(buf, bufSize, indexStart);
    if(indexStart >= bufSize) return 0;
    uint64_t hex = 0;
    char* hex_str = (char*)malloc(sizeof(char)*(nBytes * 2 + 1));
    hex_str[nBytes] = '\0';
    for(int i = 0; i < nBytes; i++){
        if(indexStart + nBytes - i >= bufSize) return 0;
        char* temp = (char*)malloc(sizeof(char)*3);
        sprintf(temp, "%02X", buf[indexStart + (nBytes - 1) - i]);
        hex_str[i*2] = temp[0];
        hex_str[i*2 + 1] = temp[1];
        free(temp);
    }
    hex = strtol(hex_str, NULL, 16);
    free(hex_str);
    return hex;

}