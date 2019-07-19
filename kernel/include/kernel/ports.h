#ifndef PORTS_H
#define PORTS_H

#include <stddef.h>
#include <stdint.h>

unsigned char port_byte_in (uint16_t port);
void port_byte_out (uint16_t port, uint8_t data);
unsigned short port_word_in (uint16_t port);
void port_word_out (uint16_t port, uint16_t data);
void port_insw (uint16_t port, void *addr, size_t cnt);

#endif
