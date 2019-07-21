#include <kernel/ata.h>
#include <kernel/ports.h>
#include <stdio.h>
#include <stdlib.h>


/**
;       Reading the harddisk using ports!
;       +-------------------------------+   by qark
;
;
;  This took me months to get working but I finally managed it.
;
;  This code only works for the 286+ so you must detect for 8088's somewhere
;  in your code.
;
;  Technical Information on the ports:
;      Port    Read/Write   Misc
;     ------  ------------ -------------------------------------------------
;       1f0       r/w       data register, the bytes are written/read here
;       1f1       r         error register  (look these values up yourself)
;       1f2       r/w       sector count, how many sectors to read/write
;       1f3       r/w       sector number, the actual sector wanted
;       1f4       r/w       cylinder low, cylinders is 0-1024
;       1f5       r/w       cylinder high, this makes up the rest of the 1024
;       1f6       r/w       drive/head
;                              bit 7 = 1
;                              bit 6 = 0  LBA mode
;                              bit 5 = 1
;                              bit 4 = 0  drive 0 select
;                                    = 1  drive 1 select
;                              bit 3-0    head select bits
;       1f7       r         status register
;                              bit 7 = 1  controller is executing a command
;                              bit 6 = 1  drive is ready
;                              bit 5 = 1  write fault
;                              bit 4 = 1  seek complete
;                              bit 3 = 1  sector buffer requires servicing
;                              bit 2 = 1  disk data read corrected
;                              bit 1 = 1  index - set to 1 each revolution
;                              bit 0 = 1  previous command ended in an error
;       1f7       w         command register
;                            commands:
;                              50h format track
;                              20h read sectors with retry
;                              21h read sectors without retry
;                              22h read long with retry
;                              23h read long without retry
;                              30h write sectors with retry
;                              31h write sectors without retry
;                              32h write long with retry
;                              33h write long without retry
;
;  Most of these should work on even non-IDE hard disks.
;  This code is for reading, the code for writing is the next article.
*/

void ata_delay() {
    port_byte_in(0x1f7);
    port_byte_in(0x1f7);
    port_byte_in(0x1f7);
    port_byte_in(0x1f7);
}

void ata_soft_reset() {
    port_byte_out(0x3f6, 0x04);
    ata_delay();
    port_byte_out(0x3f6, 0x0);
}

//Taken from OSDev wiki
uint8_t detect_device_type(uint8_t drive_num) {
    ata_soft_reset();
    set_drive_and_head(drive_num);
    unsigned cl = port_byte_in(0x1f4);
    unsigned ch = port_byte_in(0x1f5);

    /* differentiate ATA, ATAPI, SATA and SATAPI */
    if (cl==0x14 && ch==0xEB) return ATADEV_PATAPI;
    if (cl==0x69 && ch==0x96) return ATADEV_SATAPI;
    if (cl==0 && ch == 0) return ATADEV_PATA;
    if (cl==0x3c && ch==0xc3) return ATADEV_SATA;
    return ATADEV_UNKNOWN;
}


void set_drive_and_head(uint8_t drive_head_mask) {
    port_byte_out(0x1f6, drive_head_mask);
    ata_delay();
}

uint8_t ata_identify(uint8_t drive_num) {
    printf("Start IDENTIFY\n");
    uint8_t dt = detect_device_type(drive_num);
    printf("  Block Device type: %d\n", dt);
    set_sector_count(0);
    set_sector_number(0);
    set_cylinder(0);
    port_byte_out(0x1f7, 0xec);
    printf("  IDENTIFY sent\n");
    ata_delay();
    uint8_t disk_status = port_byte_in(0x1f7);
    if (disk_status) {
        if ((dt != ATADEV_PATAPI) || (dt != ATADEV_SATAPI)) {
            while((port_byte_in(0x1f7) & 0x80) != 0);
        }

pm_stat_read:    disk_status = port_byte_in(0x1f7);

        if (disk_status & 0x01) {
            printf("Failed IDENTIFY. Disk ERR\n");
            return 1;
        }
        while(!(disk_status & 0x08)) goto pm_stat_read;
        printf("  Device Active\n");
    } else {
        printf("Failed IDENTIFY. Disk Not Found\n");
        return 1;
    }
    printf("Finished IDENTIFY\n");
    return 0;
}

void set_sector_count(uint8_t count) {
    port_byte_out(0x1f2, count);
}

void set_sector_number(uint8_t number) {
    port_byte_out(0x1f3, number);
}

void set_cylinder(uint8_t cylinder) {
    port_byte_out(0x1f4, cylinder);
    //TODO Add logic for cyl > 255
    port_byte_out(0x1f5, 0);
}

void set_read_command() {
    port_byte_out(0x1f7, 0x20);
}

void set_lba28_triplet(uint32_t lba) {
    // LBA low
    port_byte_out(0x1f3, (unsigned char)lba);
    // LBA mid
    port_byte_out(0x1f4, (unsigned char)(lba >> 8));
    // LBA high
    port_byte_out(0x1f5, (unsigned char)(lba >> 16));
}

void lba_read_sectors(uint32_t lba, uint8_t count, void *addr) {
    uint8_t drive_head = port_byte_in(0x1f6);
    // Enable LBA mode
    drive_head = drive_head | 0x40;
    // Set 4 high bits of LBA
    drive_head = drive_head | ((lba >> 24) & 0x0f);
    set_drive_and_head(drive_head);
    ata_delay();
    set_sector_count(count);
    set_lba28_triplet(lba);
    set_read_command();
    read_words(addr);
    ata_delay();
}

void read_words(void *addr) {
    printf("DEBUG: Reading from disk\n");
    uint8_t disk_status = port_byte_in(0x1f7);
    while (disk_status & 0x80) {
        printf("DEBUG: disk is busy\n");
        disk_status = port_byte_in(0x1f7);
    }
    while (!(disk_status & 8) ) {
        printf("DEBUG: Waiting for data ready\n");
        disk_status = port_byte_in(0x1f7);
    }
    port_insw(0x1f0, addr, 256);
    ata_delay();
}

mbr_pt_entry_t *mbr_read_pt(uint8_t drive_num) {
    uint8_t t[512];
    set_drive_and_head(drive_num);
    set_sector_count(1);
    set_cylinder(0);
    set_sector_number(1);
    set_read_command();
    ata_delay();
    read_words(t);
    mbr_pt_entry_t *pt = malloc(4*sizeof(mbr_pt_entry_t));
    mbr_pt_entry_t *pt_start = pt;
    int offset;
    int k;
    uint8_t tmp[4];
    for (int i = 0; i < 4; i++) {
        offset = MBR_PT_BASE_OFFSET + i*MBR_ENTRY_SIZE;
        if (t[offset+4] == 0) {
          continue;
        }
        pt->boot = t[offset];
        pt->s_head = t[offset+1];
        pt->s_sector = t[offset+2] & 0x3f;
        pt->s_cylinder = ((t[offset+2] >> 6) << 8) | t[offset+3];
        pt->sys_id = t[offset+4];
        pt->e_head = t[offset+5];
        pt->e_sector = t[offset+6] & 0x3f;
        pt->e_cylinder = ((t[offset+6] >> 6) << 8) | t[offset+7];
        uint8_t tmp[4] = {t[offset+8], t[offset+9], t[offset+10], t[offset+11]};    
        SLICE(t,tmp,offset+8,4,k);
        pt->relative_sector = byte2long(tmp);
        SLICE(t,tmp,offset+12,4,k);
        pt->total_sectors = byte2long(tmp);
        pt++;
    }
    return pt_start;
} 

