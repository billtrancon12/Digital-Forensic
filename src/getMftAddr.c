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
#include "getMftAddr.h"

uint64_t getMftAddr(int fd, uint64_t mbr_addr){
    off64_t lseekMFT = lseek64(fd, mbr_addr, SEEK_SET);
    if(lseekMFT == errno){
        printf("%s\n",strerror(errno));
        return EXIT_FAILURE;
    }

    struct disk_ntfs_br* br_obj = (struct disk_ntfs_br*)malloc(sizeof(struct disk_ntfs_br));
    int readVal = read(fd, br_obj, 512);
    if(readVal == errno){
        printf("%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

	uint32_t cluster_size = br_obj->bpb_table.byte_per_sector * br_obj->bpb_table.sectors_per_cluster;
	uint64_t hex_addr = mbr_addr + cluster_size * br_obj->bpb_table.logical_cluster_mft;

    return hex_addr;
}
