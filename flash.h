/******************************************************************************
 * File:        flash.h
 * Description: Public interface for high-level ESP32 flash write operations and progress callbacks.
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stddef.h>
#include "serial.h"

/* ---------------------------------------------------------
 * Callback typedefs MUST come first
 * --------------------------------------------------------- */
typedef void (*flash_progress_cb_t)(uint32_t bytes_written,
                                    uint32_t total_bytes);

typedef void (*flash_encrypt_cb_t)(uint32_t addr,
                                   uint8_t *data,
                                   size_t len);

/* ---------------------------------------------------------
 * Flash image descriptor
 * --------------------------------------------------------- */
typedef struct {
    const char *file;                 /* path to firmware file */
    uint32_t addr;                    /* base flash address */
    flash_progress_cb_t progress_cb;  /* optional progress callback */
    flash_encrypt_cb_t  encrypt_cb;   /* optional encryption callback */
} flash_image_t;


/* ---------------------------------------------------------
 * API
 * --------------------------------------------------------- */
// int flash_write(serial_handle_t *sh, const flash_image_t *img);
int flash_write(serial_handle_t *sh, flash_image_t *img);

#endif
