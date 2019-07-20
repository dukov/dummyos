#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UNUSED(x) (void)(x)
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define HEAP_END 0x800000

typedef struct m_record {
    uint32_t size;
    struct m_record *next;
} m_record_t;

#if defined(__is_libk)
void mm_init();
#endif

__attribute__((__noreturn__))
void abort(void);

void* memset(void*, int, size_t);
char* malloc(size_t size);
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
