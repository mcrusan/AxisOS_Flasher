/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher)
 *  File    : bootrom_cmd.h
 *  Author  : Marc / AxisOS
 *  Purpose : Defines ESP32 ROM bootloader command IDs and basic command types.
 ******************************************************************************/

#ifndef AXFLASH_BOOTROM_CMD_H
#define AXFLASH_BOOTROM_CMD_H

#include <stdint.h>

/******************************************************************************
 *  ROM Command IDs
 *  (Values based on ESP32 ROM bootloader protocol)
 *
 *  NOTE:
 *    - All IDs must be unique.
 *    - Grouped by functional domain.
 *    - Custom AxisOS extensions live in the 0xB0–0xBF range.
 ******************************************************************************/

/*** Flashing Commands *******************************************************/
#define BR_CMD_FLASH_BEGIN          0x02
#define BR_CMD_FLASH_DATA           0x03
#define BR_CMD_FLASH_END            0x04

/*** Memory Commands *********************************************************/
#define BR_CMD_MEM_BEGIN            0x05
#define BR_CMD_MEM_END              0x06
#define BR_CMD_MEM_DATA             0x07

/*** Sync & Register Commands ************************************************/
#define BR_CMD_SYNC                 0x08
#define BR_CMD_WRITE_REG            0x09
#define BR_CMD_READ_REG             0x0A

/*** SPI / Baud / Attach *****************************************************/
#define BR_CMD_SPI_ATTACH           0x0D
#define BR_CMD_CHANGE_BAUD          0x0F

/*** Deflated Flashing *******************************************************/
#define BR_CMD_FLASH_DEFL_BEGIN     0x10
#define BR_CMD_FLASH_DEFL_DATA      0x11
#define BR_CMD_FLASH_DEFL_END       0x12

/*** ESP32-Specific Extended Commands ****************************************/
#define BR_CMD_FLASH_ERASE_REGION   0xD0
#define BR_CMD_FLASH_MD5            0xD1

/*** AxisOS Custom Extensions *************************************************
 *  Reserved range: 0xB0–0xBF
 *  These do NOT collide with official ROM commands.
 ******************************************************************************/
#define BR_CMD_RESET_TO_RUN         0xB0   /* AxisOS: reset + run application */

/******************************************************************************
 *  Command Result Codes (high-level)
 ******************************************************************************/

typedef enum {
    BR_OK = 0,
    BR_ERR_IO,
    BR_ERR_TIMEOUT,
    BR_ERR_BAD_RESPONSE,
    BR_ERR_PROTOCOL,
} br_result_t;

#endif /* AXFLASH_BOOTROM_CMD_H */
