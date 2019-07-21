#include <stdlib.h>

void push(void *ptr, void *new_element, uint32_t len) {
    ptr = realloc(ptr, sizeof(ptr[0])*len + sizeof(ptr[0]));
    memcpy(&ptr[len], new_element, sizeof(ptr[0]));
}
