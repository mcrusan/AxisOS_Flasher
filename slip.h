/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher) 🍌
 *  File    : slip.h
 *  Author  : Marc / AxisOS
 *  Purpose : Implements SLIP (Serial Line Internet Protocol) encoding and
 *            decoding for ESP32 ROM bootloader packets. Handles framing,
 *            escaping, and buffer management.
 ******************************************************************************/

#ifndef AXFLASH_SLIP_H
#define AXFLASH_SLIP_H

#include <stdint.h>
#include <stddef.h>

/* SLIP special bytes */
#define SLIP_END       0xC0
#define SLIP_ESC       0xDB
#define SLIP_ESC_END   0xDC
#define SLIP_ESC_ESC   0xDD

/* Encode raw data into SLIP frame */
size_t slip_encode(uint8_t *dst,
                   const uint8_t *src,
                   size_t len);

/* Decode SLIP frame into raw data */
size_t slip_decode(uint8_t *dst,
                   const uint8_t *src,
                   size_t len);

#endif /* AXFLASH_SLIP_H */
