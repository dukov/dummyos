#include <stdlib.h>
#include <kernel/ramfs.h>
#include <kernel/ata.h>

unsigned int getsize(const char *in)
{
 
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
 
}

uint32_t get_next_sector(uint32_t cur_sect_lba, struct tar_header *header) {
    unsigned int address = 0;
    unsigned int size = getsize(header->size);
    address += ((size / 512) + 1) * 512;
    if (size % 512)
        address += 512;
    return cur_sect_lba + address/512;
}


tar_header_t *load_ramfs(uint32_t lba, uint32_t *count) {
    char t[512];
    unsigned int i;
    uint32_t next_sect_lba = lba;
    for (i = 0; ; i++)
    {
        lba_read_sectors(next_sect_lba, 1, t);
        struct tar_header *header = (struct tar_header *)t;
        if (header->filename[0] == '\0')
            break;
        if (ramfs_headers == NULL) {
            ramfs_headers = (tar_header_t *)malloc(sizeof(tar_header_t));
            ramfs_headers = memcpy(ramfs_headers, header, sizeof(tar_header_t));
        } else {
//            ramfs_headers = realloc(ramfs_headers, (i+1)*sizeof(tar_header_t));
//            ramfs_headers[i] = *((tar_header_t *) memcpy((void *)(&ramfs_headers[i]), header, sizeof(tar_header_t)));
            ramfs_headers = push(ramfs_headers, header, i, sizeof(tar_header_t));
        }
        
        next_sect_lba = get_next_sector(next_sect_lba, header);
     }
     *count = i;
     return ramfs_headers;
}
