#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UNUSED(x) (void)(x)
#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)
__attribute__((__noreturn__))
void abort(void);

#ifdef __cplusplus
}
#endif

#endif
