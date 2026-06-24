/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher)
 *  File    : flash.c
 *  Author  : Marc / AxisOS
 *  Purpose : High-level flash write orchestration. Handles chunking, offsets,
 *            block sequencing, and delegates ROM operations to bootrom.c.
 ******************************************************************************/

#include "flash.h"
#include "bootrom.h"
#include "serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLASH_BLOCK_SIZE 4096

static void encrypt_stub(uint32_t addr, uint8_t *data, size_t len);


/******************************************************************************
 *  Flash Write
 ******************************************************************************/
// int flash_write(serial_handle_t *sh, const flash_image_t *img)
int flash_write(serial_handle_t *sh, flash_image_t *img)
{

    if (!img->encrypt_cb)
	img->encrypt_cb = encrypt_stub;


    if (!img || !img->file)
        return 0;

    FILE *fp = fopen(img->file, "rb");
    if (!fp)
        return 0;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (bootrom_flash_erase(sh, img->addr, (uint32_t)size) != BR_OK) {
        fclose(fp);
        return 0;
    }

    if (bootrom_flash_begin(sh,
                            (uint32_t)size,
                            img->addr,
                            FLASH_BLOCK_SIZE) != BR_OK)
    {
        fclose(fp);
        return 0;
    }


    uint8_t buffer[FLASH_BLOCK_SIZE];
    size_t n;
    uint32_t written  = 0;
    uint32_t cur_addr = img->addr;

    while ((n = fread(buffer, 1, FLASH_BLOCK_SIZE, fp)) > 0) {

        /* 🔐 Optional encryption hook */
        if (img->encrypt_cb)
            img->encrypt_cb(cur_addr, buffer, n);

	// 20260623 (MFC) Flash data prog fix
        // if (bootrom_flash_data(sh, buffer, n) != BR_OK) {
	if (bootrom_flash_data(sh, buffer, n) != BR_OK) {

            fclose(fp);
            return 0;
        }

        written  += n;
        cur_addr += n;

        if (img->progress_cb)
            img->progress_cb(written, (uint32_t)size);
    }

    fclose(fp);

    if (bootrom_flash_end(sh, 0) != BR_OK)
        return 0;

    return 1;
}

static void encrypt_stub(uint32_t addr, uint8_t *data, size_t len)
{
    (void)addr;
    /* TODO: replace with real ESP32-compatible encryption */
    for (size_t i = 0; i < len; i++)
        data[i] ^= 0xA5;  /* placeholder */
}
