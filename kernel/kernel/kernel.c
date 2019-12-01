#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/isr.h>
#include <kernel/ata.h>
#include <kernel/ramfs.h>

void ramfs_print(unsigned int count, tar_header_t *rfs_hdrs) {
    for (unsigned int i = 0;i<count; i++) {
        printf("File %s", rfs_hdrs[i].filename);
    }
     
}

void mbr_pt_print(mbr_pt_entry_t *ptbl) {
    printf("Partition table\n");
    mbr_pt_entry_t* iter = ptbl;
    for (int i = 0; i < 4; i++) {
        if (iter->sys_id) {
            printf("Part %d Start LBA: %d | Num Sectors: %d\n", i, iter->relative_sector, iter->total_sectors);
        }
        iter++;
    }
}

void kernel_main(void) {
    terminal_initialize();
    isr_install();
    irq_install();
    asm volatile("sti");
    printf("Hello, kernel World!\n");
//    asm("int $1");
    mm_init();
    mbr_pt_entry_t *mbr_pt = mbr_read_pt(0xa0); 
    mbr_pt_print(mbr_pt);
    unsigned int len = 0;
    tar_header_t *rfs = load_ramfs(2048, &len);
    ramfs_print(len, rfs);
    for(;;) {
        asm("hlt");
    }
}

