#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
extern void *kernel_end;
uint32_t free_mem_addr = 0;
uint32_t heap_begin = 0;
uint32_t heap_end = 0;

void mm_init() {
    free_mem_addr = (uint32_t)(&kernel_end) + 0x1000;
    heap_begin = free_mem_addr;
    heap_end = HEAP_END;
    //Zeroing first record
    memset((void *)heap_begin, 0, sizeof(m_record_t));
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

m_record_t *create_mr(m_record_t *cur_mr, m_record_t *next, size_t size) {
    m_record_t *new_mr = (m_record_t *)(
        (uint32_t)cur_mr + 
        sizeof(m_record_t) + 
        cur_mr->size
    );
    new_mr->size = size;
    new_mr->next = next;
    cur_mr->next = new_mr;
    return new_mr;
}

char* malloc(size_t size) {
    uint32_t cur_rec = heap_begin;
    m_record_t *prev_rec;
    while (cur_rec < heap_end) {
        m_record_t *mr = (m_record_t *)cur_rec;
        if (mr->next != NULL) {
            uint32_t hole_size = (uint32_t)mr->next - ((uint32_t)mr + sizeof(m_record_t) + mr->size);
            if (size + sizeof(m_record_t) <= hole_size) {
                m_record_t *new_mr = create_mr(mr, mr->next, size);
                char *res = (char *)((uint32_t)new_mr + (sizeof(m_record_t)));
                memset(res, 0, size);
                return res;
            }
            cur_rec = (uint32_t)(mr->next);
        } else if (mr->next == NULL && mr->size != 0) {
                m_record_t *new_mr = create_mr(mr, NULL, size);
                char *res = (char *)((uint32_t)new_mr + (sizeof(m_record_t)));
                memset(res, 0, size);
                return res;
        } else {
            mr->size = size;
            mr->next = NULL;
            return (char *)(cur_rec + (sizeof(m_record_t)));
        }
    }
    return NULL;
}

void free(void *ptr) {
    m_record_t *mr = ((uint32_t)ptr) - sizeof(m_record_t);
    if ((uint32_t)mr == heap_begin) {
        memset((uint8_t *)mr, 0, sizeof(m_record_t));
        ptr = NULL;
    } else {
        uint32_t cur_mr_addr = (uint32_t)(mr) - 1;
        while (cur_mr_addr >= heap_begin) {
            m_record_t *cur_mr = (m_record_t *)cur_mr_addr;
            if (cur_mr->next == mr) {
                cur_mr->next = mr->next;
                ptr = NULL;
            }
            cur_mr_addr--;
        }
    }
}
#endif
