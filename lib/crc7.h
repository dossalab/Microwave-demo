#ifndef LIB_CRC7_BE_H
#define LIB_CRC7_BE_H

#include <stddef.h>
#include <stdint.h>

uint8_t crc7_be(uint8_t crc, const uint8_t *buffer, size_t len);

#endif

