#ifndef RAMFS_H
#define RAMFS_H

#include <stdint.h>

struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};

typedef struct tar_header tar_header_t; 

struct dir_tree
{
   tar_header_t *hdr;
   struct dir_tree *childs;
   uint32_t child_num;
};

static tar_header_t *ramfs_headers = NULL;

tar_header_t *load_ramfs(uint32_t sect_num, uint32_t *count);

#endif
