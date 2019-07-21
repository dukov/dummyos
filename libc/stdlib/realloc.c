#include <stdlib.h>

void *realloc(void *ptr, size_t size) {
    if (!size) {
        free(ptr);
        return NULL;
    }
    if (!ptr) {
        return malloc(size);
    }
    void *res = malloc(size);
    m_record_t *mr = (m_record_t *)((uint32_t)ptr - sizeof(m_record_t));
    res = memcpy(res, ptr, mr->size);
    free(ptr);
    return res;
}
