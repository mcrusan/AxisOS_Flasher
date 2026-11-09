/******************************************************************************
 * File:        slip.h
 * Description: Public interface for SLIP packet encoding and decoding routines. 🍌
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

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
