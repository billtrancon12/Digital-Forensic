#ifndef __NTFSBR__

#define __NTFSBR__
#include <stdint.h>

typedef struct bpb_and_extended_bpb_table
{
    uint16_t byte_per_sector;
    uint8_t sectors_per_cluster;
    uint8_t not_used[7];
    uint8_t media_descriptor;
    uint8_t not_used2[18];
    uint64_t total_sectors;
    uint64_t logical_cluster_mft;
    uint64_t logical_copy_cluster_mft;
    uint32_t clusters_per_file_record_segment;
    uint8_t cluster_per_index_buffer;
    uint8_t not_used3[3];
    uint64_t volume_serial_number;
    uint32_t checksum;
} __attribute__((packed)) BPB_and_extended_BPB_table;

typedef struct disk_ntfs_br
{
    uint8_t jump_instruction[3];
    uint64_t OEM_ID;
    BPB_and_extended_BPB_table bpb_table;
    uint8_t bootstrap_code[426];
    uint8_t end_sector_marker[2];
} __attribute__((packed)) DISK_ntfs_br;

#endif