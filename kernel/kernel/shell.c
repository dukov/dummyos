#include <kernel/shell.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *shell_cmds[] = {
    "END",
//    "PAGE",
//    "READALL",
//    "RAMFS",
//    "PT",
//    "MALLOC",
};


static shell_function_t shell_functions[] = {
    &shell_end,
//    &shell_page,
//    &shell_read_all_sectors,
//    &shell_load_ramfs,
//    &shell_read_pt,
//    &shell_malloc
};

void shell_run_command(char *input) {
    for(unsigned int i = 0; i < LEN(shell_cmds); i++ ) {
        if (strcmp(input, shell_cmds[i]) == 0) {
            shell_functions[i]();
            printf("\n > ");
            return;
        }
    }
    printf("No such command\n");
    printf("\n > ");
}

void shell_end() {
    printf("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
}
/*
void shell_page() {
    uint32_t phys_addr;
    uint32_t page = kmalloc(1000, &phys_addr);
    char page_str[16] = "";
    hex_to_ascii(page, page_str);
    char phys_str[16] = "";
    hex_to_ascii(phys_addr, phys_str);
    kprint("Page: ");
    kprint(page_str);
    kprint(", physical address: ");
    kprint(phys_str);
    kprint("\n");
}

void shell_read_all_sectors() {
    char t[512];
    set_drive_and_head(0x0a0);
    set_sector_count(1);
    set_cylinder(0);
    for (uint8_t i = 1; i < 50; i++) {
        set_sector_number(i);
        set_read_command();
        read_words(t);
    }
}

void shell_load_ramfs() {
    load_ramfs(27);
}

void shell_read_pt() {
    mbr_read_pt(0xa0);
}

void shell_malloc() {
    void *ptr = malloc(sizeof(uint16_t));    
}
*/
