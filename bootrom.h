/******************************************************************************
 * File:        bootrom.h
 * Description: Public interface for ESP32 ROM bootloader protocol operations.
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#ifndef AXFLASH_BOOTROM_H
#define AXFLASH_BOOTROM_H

#include <stdint.h>
#include <stddef.h>
#include "serial.h"
#include "bootrom_cmd.h"

/******************************************************************************
 *  High-level ROM Operations
 ******************************************************************************/

 br_result_t bootrom_reg_write(serial_handle_t *sh,
                              uint32_t addr,
                              uint32_t value,
                              uint32_t mask);

br_result_t bootrom_reg_read(serial_handle_t *sh,
                             uint32_t addr,
                             uint32_t *out_value);
 
br_result_t bootrom_flash_md5(serial_handle_t *sh,
                              uint32_t offset,
                              uint32_t size,
                              uint8_t out_md5[16]);

br_result_t bootrom_flash_erase(serial_handle_t *sh,
                                uint32_t offset,
                                uint32_t size);

br_result_t bootrom_sync(serial_handle_t *sh);

br_result_t bootrom_flash_begin(serial_handle_t *sh,
                                uint32_t size,
                                uint32_t offset,
                                uint32_t block_size);

// 20260623 (MFC) Interface change and back again and again
br_result_t bootrom_flash_data(serial_handle_t *sh,
                               const uint8_t *data,
                               size_t len);



br_result_t bootrom_flash_end(serial_handle_t *sh, int stay_in_bootloader);

int bootrom_send_cmd(serial_handle_t *sh,
	                     uint8_t cmd,
	                     const void *payload,	
	                     uint32_t len);
 

int bootrom_reset_to_run(serial_handle_t *sh);

void bootrom_reset(serial_handle_t *sh);

void bootrom_enter_bootloader(serial_handle_t *sh);


#endif /* AXFLASH_BOOTROM_H */
