/******************************************************************************
 * File:        bootrom_pkt.h
 * Description: Packet-level interface for ESP32 ROM bootloader SLIP framing, checksums, and response parsing. 🍌🐒
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#ifndef AXFLASH_BOOTROM_PKT_H
#define AXFLASH_BOOTROM_PKT_H

#include <stdint.h>
#include <stddef.h>

#include "serial.h"
#include "bootrom_cmd.h"
#include "slip.h"

/******************************************************************************
 *  ESP32 Packet Header
 *
 *  struct {
 *      uint8_t  direction;   // 0x00 host → device
 *      uint8_t  command;     // BR_CMD_*
 *      uint16_t size;        // payload size
 *      uint32_t checksum;    // XOR checksum of payload
 *  }
 ******************************************************************************/

typedef struct {
    uint8_t  direction;
    uint8_t  command;
    uint16_t size;
    uint32_t checksum;
} br_pkt_header_t;

/******************************************************************************
 *  ROM Response Structure
 ******************************************************************************/

typedef struct {
    uint8_t  resp_cmd;
    uint8_t  status;
    uint8_t  value[4];
} br_response_t;

/******************************************************************************
 *  API
 ******************************************************************************/

br_result_t br_pkt_send_cmd(serial_handle_t *sh,
                            uint8_t cmd,
                            const void *payload,
                            uint16_t payload_len,
                            br_response_t *out_resp,
                            int timeout_ms);

uint32_t br_pkt_checksum32(const uint8_t *data, size_t len);

#endif /* AXFLASH_BOOTROM_PKT_H */
