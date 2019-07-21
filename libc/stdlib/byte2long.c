#include <stdlib.h>

uint32_t byte2long(uint8_t byte_array[4]) {
    uint32_t res = byte_array[3];
    uint8_t k = 0;
    while (k < 3) {
        res = (res << 8) | byte_array[2-k];
        k++;
    }
    return res;
}
