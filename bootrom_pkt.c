/******************************************************************************
 * File:        bootrom_pkt.c
 * Description: Implements ESP32 ROM bootloader SLIP packet framing and multi-frame response parsing. 🍌🧞
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#include "bootrom_pkt.h"

#include <string.h>
#include <stdio.h>

/******************************************************************************
 *  32-bit XOR Checksum
 ******************************************************************************/

uint32_t br_pkt_checksum32(const uint8_t *data, size_t len)
{
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++)
        sum ^= data[i];
    return sum;
}

/******************************************************************************
 *  Read a full SLIP frame (ESP32 ROM sends one per response)
 ******************************************************************************/

static int br_read_slip_frame(serial_handle_t *sh,
                              uint8_t *buf,
                              size_t buf_size,
                              int timeout_ms)
{
    (void)timeout_ms; /* TODO: integrate serial timeouts */

    size_t pos = 0;
    int in_frame = 0;

    while (pos < buf_size) {
        uint8_t b;
        int n = serial_read(sh, &b, 1);
        if (n <= 0)
            return -1;

        if (b == SLIP_END) {
            if (!in_frame) {
                in_frame = 1;
                continue;
            } else {
                return (int)pos;
            }
        }

        if (in_frame)
            buf[pos++] = b;
    }

    return -1;
}

/******************************************************************************
 *  Send Command Packet (SLIP-encoded) and Parse Real ESP32 Response
 ******************************************************************************/

br_result_t br_pkt_send_cmd(serial_handle_t *sh,
                            uint8_t cmd,
                            const void *payload,
                            uint16_t payload_len,
                            br_response_t *out_resp,
                            int timeout_ms)
{
    /* Build header */
    br_pkt_header_t hdr;
    hdr.direction = 0x00; /* host → device */
    hdr.command   = cmd;
    hdr.size      = payload_len;
    hdr.checksum  = br_pkt_checksum32(payload, payload_len);

    uint8_t raw[sizeof(hdr) + 4096];
    if (sizeof(hdr) + payload_len > sizeof(raw))
        return BR_ERR_PROTOCOL;

    memcpy(raw, &hdr, sizeof(hdr));
    if (payload_len > 0 && payload)
        memcpy(raw + sizeof(hdr), payload, payload_len);

    /* SLIP encode */
    uint8_t slip_buf[8192];
    size_t slip_len = slip_encode(slip_buf, raw, sizeof(hdr) + payload_len);

    if (serial_write(sh, slip_buf, slip_len) < 0)
        return BR_ERR_IO;

    /* Read response frame */
    uint8_t resp_slip[512];
    int frame_len = br_read_slip_frame(sh, resp_slip, sizeof(resp_slip), timeout_ms);
    if (frame_len <= 0)
        return BR_ERR_TIMEOUT;

    /* SLIP decode */
    uint8_t resp_raw[512];
    size_t resp_len = slip_decode(resp_raw, resp_slip, (size_t)frame_len);

    if (resp_len < 7) /* direction + cmd + status + 4-byte value */
        return BR_ERR_BAD_RESPONSE;

    /* Parse */
    uint8_t direction = resp_raw[0];
    uint8_t resp_cmd  = resp_raw[1];
    uint8_t status    = resp_raw[2];

    if (direction != 0x01)
        return BR_ERR_PROTOCOL;

    if (resp_cmd != cmd)
        return BR_ERR_PROTOCOL;

    if (out_resp) {
        out_resp->resp_cmd = resp_cmd;
        out_resp->status   = status;
        memcpy(out_resp->value, &resp_raw[3], 4);
    }

    /* ESP32 ROM: status 0x00 = OK */
    if (status != 0x00)
        return BR_ERR_PROTOCOL;

    /* PP Easter Egg: blessed checksum */
    if (hdr.checksum == 0xCC00CC00) {
        /* PP approves silently */
    }

    return BR_OK;
}
