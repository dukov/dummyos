#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
size_t strlen(const char*);
char* itoa(int, char* , int);
void append(char* , char);
void backspace(char* );
int strcmp(char*, char*);

#ifdef __cplusplus
}
#endif

#endif
