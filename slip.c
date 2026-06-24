/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher) 🍌
 *  File    : slip.c
 *  Author  : Marc / AxisOS
 *  Purpose : Implements SLIP encoding and decoding for ESP32 ROM bootloader
 *            packets. Provides clean, minimal routines for packet framing.
 ******************************************************************************/

#include "slip.h"

/******************************************************************************
 *  SLIP Encode
 ******************************************************************************/

size_t slip_encode(uint8_t *dst,
                   const uint8_t *src,
                   size_t len)
{
    size_t out = 0;

    dst[out++] = SLIP_END;

    for (size_t i = 0; i < len; i++) {
        uint8_t b = src[i];

        switch (b) {
            case SLIP_END:
                dst[out++] = SLIP_ESC;
                dst[out++] = SLIP_ESC_END;
                break;

            case SLIP_ESC:
                dst[out++] = SLIP_ESC;
                dst[out++] = SLIP_ESC_ESC;
                break;

            default:
                dst[out++] = b;
                break;
        }
    }

    dst[out++] = SLIP_END;
    return out;
}

/******************************************************************************
 *  SLIP Decode
 ******************************************************************************/

size_t slip_decode(uint8_t *dst,
                   const uint8_t *src,
                   size_t len)
{
    size_t out = 0;
    int esc = 0;

    for (size_t i = 0; i < len; i++) {
        uint8_t b = src[i];

        if (b == SLIP_END) {
            /* Frame boundary — ignore for now */
            continue;
        }

        if (esc) {
            switch (b) {
                case SLIP_ESC_END: dst[out++] = SLIP_END; break;
                case SLIP_ESC_ESC: dst[out++] = SLIP_ESC; break;
                default:           dst[out++] = b;       break;
            }
            esc = 0;
            continue;
        }

        if (b == SLIP_ESC) {
            esc = 1;
            continue;
        }

        dst[out++] = b;
    }

    return out;
}
