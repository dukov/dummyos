#ifndef DISK_H
#define DISK_H

#define MBR_PT_BASE_OFFSET 446
#define MBR_ENTRY_SIZE 16

/* device types */
#define ATADEV_NODEVICE 0x0
#define ATADEV_PATA     0x1
#define ATADEV_PATAPI   0x2
#define ATADEV_SATA     0x3
#define ATADEV_SATAPI   0x4
#define ATADEV_UNKNOWN  0x5

#include <stdint.h>

struct mbr_pt_entry {
    uint8_t boot;
    uint8_t s_head;
    uint8_t s_sector;
    uint16_t s_cylinder;
    uint8_t sys_id;
    uint8_t e_head;
    uint8_t e_sector;
    uint16_t e_cylinder;
    uint32_t relative_sector;
    uint32_t total_sectors;
};

typedef struct mbr_pt_entry mbr_pt_entry_t;

void set_drive_and_head(uint8_t drive_head_mask);
uint8_t ata_identify(uint8_t drive_num);
void set_sector_count(uint8_t count);
void set_sector_number(uint8_t number);
void set_cylinder(uint8_t cylinder);
void set_read_command();
void lba_read_sectors(uint32_t lba, uint8_t count, void *addr);
void read_words(void *addr);
mbr_pt_entry_t *mbr_read_pt(uint8_t drive_num);
#endif
