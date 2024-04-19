#include <stdint.h>

typedef struct nonResNamedAttr
{
    uint32_t type;
    uint32_t len;
    uint8_t resident_flag;
    uint8_t name_len;
    uint16_t offset_name;
    uint16_t flags;     // Currently used or deleted
    uint16_t id;
    uint64_t startVCN;
    uint64_t lastVCN;
    uint16_t offset_data_run;
    uint16_t compression_unit_size;
    uint32_t padding;
    uint64_t allocated_size;
    uint64_t real_size;
    uint64_t data_size_stream;
} __attribute__((packed)) no_resident_named_attr;

typedef struct nonResNonNameAttr
{
    uint32_t type;
    uint32_t len;
    uint8_t resident_flag;
    uint8_t name_len;
    uint16_t offset_name;
    uint16_t flags;
    uint16_t id;
    uint64_t startVCN;
    uint64_t lastVCN;
    uint16_t offset_data_run;
    uint16_t compression_unit_size;
    uint32_t padding;
    uint64_t allocated_size;
    uint64_t real_size;
    uint64_t data_size_stream;
} __attribute__((packed)) no_resident_no_name_attr;

typedef struct resNonNameAttr
{
    uint32_t type;
    uint32_t len;
    uint8_t resident_flag;
    uint8_t name_len;
    uint16_t offset_name;
    uint16_t flags;
    uint16_t id;
    uint32_t attr_len;
    uint16_t offset_attr;
    uint8_t indexed_flag;
    uint8_t padding;
} __attribute__((packed)) resident_no_name_attr;

