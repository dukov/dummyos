#include <stdlib.h>

void* push(void *ptr, void *new_element, uint32_t len, size_t element_size) {
    ptr = realloc(ptr, element_size*len + element_size);
    uint32_t offset = len*element_size;
    void *last = (void *)((uint32_t)ptr+offset);
    last = memcpy(last, new_element, element_size);
    return ptr;
}
