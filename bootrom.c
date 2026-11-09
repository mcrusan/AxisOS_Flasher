/******************************************************************************
 * File:        bootrom.c
 * Description: Implements ESP32 ROM bootloader protocol over SLIP framing with baud-rate switching support. 🧞
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#include "bootrom.h"
#include "bootrom_cmd.h"
#include "bootrom_pkt.h"
#include "serial.h"

#include <stdio.h>
#include <string.h>

/******************************************************************************
 *  Local Types
 ******************************************************************************/

/* FLASH_BEGIN payload */
typedef struct {
    uint32_t total_size;
    uint32_t num_blocks;
    uint32_t block_size;
    uint32_t offset;
} br_flash_begin_t;

/* FLASH_DATA payload header */
typedef struct {
    uint32_t seq;
    uint32_t length;
    uint32_t pad;
} br_flash_data_hdr_t;

/* CHANGE_BAUD payload */
typedef struct {
    uint32_t new_baud;
    uint32_t old_baud;   /* ROM ignores this, but we send it for completeness */
} br_change_baud_t;


/******************************************************************************
 *  Reset to Run 
 ******************************************************************************/
int bootrom_reset_to_run(serial_handle_t *sh)
{
    br_response_t resp;
    return br_pkt_send_cmd(sh,
                           BR_CMD_RESET_TO_RUN,
                           NULL,
                           0,
                           &resp,
                           1000);
}


/******************************************************************************
 *  Flash Erase Region
 ******************************************************************************/

typedef struct {
    uint32_t offset;
    uint32_t size;
} br_flash_erase_t;

br_result_t bootrom_flash_erase(serial_handle_t *sh,
                                uint32_t offset,
                                uint32_t size)
{
    br_flash_erase_t er;
    er.offset = offset;
    er.size   = size;

    br_response_t resp;
    return br_pkt_send_cmd(sh,
                           BR_CMD_FLASH_ERASE_REGION,
                           &er,
                           sizeof(er),
                           &resp,
                           5000);
}

/******************************************************************************
 *  Flash MD5 Verification
 ******************************************************************************/

typedef struct {
    uint32_t offset;
    uint32_t size;
} br_flash_md5_t;

br_result_t bootrom_flash_md5(serial_handle_t *sh,
                              uint32_t offset,
                              uint32_t size,
                              uint8_t out_md5[16])
{
    br_flash_md5_t md;
    md.offset = offset;
    md.size   = size;

    br_response_t resp;

    /* Request MD5 from ROM */
    br_result_t r = br_pkt_send_cmd(sh,
                                    BR_CMD_FLASH_MD5,
                                    &md,
                                    sizeof(md),
                                    &resp,
                                    5000);
    if (r != BR_OK)
        return r;

    /* ROM sends MD5 in a follow-up frame */
    uint8_t md5_frame[64];
    int n = serial_read(sh, md5_frame, sizeof(md5_frame));
    if (n < 16)
        return BR_ERR_BAD_RESPONSE;

    memcpy(out_md5, md5_frame, 16);
    return BR_OK;
}

/******************************************************************************
 *  Register / Memory Write
 ******************************************************************************/

typedef struct {
    uint32_t addr;
    uint32_t value;
    uint32_t mask;
} br_mem_write_t;

br_result_t bootrom_reg_write(serial_handle_t *sh,
                              uint32_t addr,
                              uint32_t value,
                              uint32_t mask)
{
    br_mem_write_t mw;
    mw.addr  = addr;
    mw.value = value;
    mw.mask  = mask;

    br_response_t resp;
    return br_pkt_send_cmd(sh,
                           BR_CMD_WRITE_REG,
                           &mw,
                           sizeof(mw),
                           &resp,
                           1000);
}

/******************************************************************************
 *  Auto-reset into ESP32 bootloader mode
 ******************************************************************************/

void bootrom_enter_bootloader(serial_handle_t *sh)
{
    /* Hold GPIO0 low (enter bootloader) */
    serial_set_dtr(sh, 1);

    /* Pulse EN low then high */
    serial_set_rts(sh, 1);
    serial_set_rts(sh, 0);

    /* Release GPIO0 */
    serial_set_dtr(sh, 0);
}

/******************************************************************************
 *  Register / Memory Read
 ******************************************************************************/

typedef struct {
    uint32_t addr;
    uint32_t length;
} br_mem_read_t;

br_result_t bootrom_reg_read(serial_handle_t *sh,
                             uint32_t addr,
                             uint32_t *out_value)
{
    if (!out_value)
        return BR_ERR_PROTOCOL;

    br_mem_read_t mr;
    mr.addr   = addr;
    mr.length = 4;   /* read one 32-bit register */

    br_response_t resp;
    br_result_t r = br_pkt_send_cmd(sh,
                                    BR_CMD_READ_REG,
                                    &mr,
                                    sizeof(mr),
                                    &resp,
                                    1000);
    if (r != BR_OK)
        return r;

    /* ESP32 ROM returns the 32-bit value in resp.value[] */
    uint32_t v;
    memcpy(&v, resp.value, sizeof(v));
    *out_value = v;

    return BR_OK;
}


/******************************************************************************
 *  Sync
 ******************************************************************************/

br_result_t bootrom_sync(serial_handle_t *sh)
{
    uint8_t sync_payload[36];
    memset(sync_payload, 0x00, sizeof(sync_payload));
    sync_payload[0] = 0x07;
    sync_payload[1] = 0x07;
    sync_payload[2] = 0x12;
    sync_payload[3] = 0x20;

    br_response_t resp;

    for (int i = 0; i < 5; i++) {
        br_result_t r = br_pkt_send_cmd(sh,
                                        BR_CMD_SYNC,
                                        sync_payload,
                                        sizeof(sync_payload),
                                        &resp,
                                        1000);
        if (r == BR_OK)
            return BR_OK;
    }

    return BR_ERR_TIMEOUT;
}

/******************************************************************************
 *  Flash Begin
 ******************************************************************************/

br_result_t bootrom_flash_begin(serial_handle_t *sh,
                                uint32_t size,
                                uint32_t offset,
                                uint32_t block_size)
{
    if (block_size == 0)
        return BR_ERR_PROTOCOL;

    uint32_t num_blocks = (size + block_size - 1) / block_size;

    br_flash_begin_t fb;
    fb.total_size = size;
    fb.num_blocks = num_blocks;
    fb.block_size = block_size;
    fb.offset     = offset;

    br_response_t resp;
    return br_pkt_send_cmd(sh,
                           BR_CMD_FLASH_BEGIN,
                           &fb,
                           sizeof(fb),
                           &resp,
                           2000);
}

/******************************************************************************
 *  Flash Data
 ******************************************************************************/
 br_result_t bootrom_flash_data(serial_handle_t *sh,
                               const uint8_t *data,
                               size_t len)
// br_result_t bootrom_flash_data(serial_handle_t *sh,
//                               const uint8_t *data,
//                               size_t len,
//                               void (*progress_cb)(uint32_t done, uint32_t total),
//                               uint32_t total_size)
{
    static uint32_t seq = 0;

    if (!data || len == 0 || len > 4096)
        return BR_ERR_PROTOCOL;

    br_flash_data_hdr_t hdr;
    hdr.seq    = seq++;
    hdr.length = (uint32_t)len;
    hdr.pad    = 0;

    uint8_t buffer[sizeof(hdr) + 4096];
    memcpy(buffer, &hdr, sizeof(hdr));
    memcpy(buffer + sizeof(hdr), data, len);

    br_response_t resp;
//    return br_pkt_send_cmd(sh,
//                           BR_CMD_FLASH_DATA,
//                           buffer,
//                           (uint16_t)(sizeof(hdr) + len),
//                           &resp,
//                           2000);
	br_result_t r = br_pkt_send_cmd(sh,
                                BR_CMD_FLASH_DATA,
                                buffer,
                                (uint16_t)(sizeof(hdr) + len),
                                &resp,
                                2000);

	if (r != BR_OK)
	    return r;

	/* Progress callback */
	// if (progress_cb) {
	//    static uint32_t bytes_written = 0;
	//    bytes_written += len;
	//    progress_cb(bytes_written, total_size);
	// }

	return BR_OK;

}

/******************************************************************************
 *  Flash End
 ******************************************************************************/

br_result_t bootrom_flash_end(serial_handle_t *sh, int stay_in_bootloader)
{
    uint32_t flag = stay_in_bootloader ? 1u : 0u;

    br_response_t resp;
    return br_pkt_send_cmd(sh,
                           BR_CMD_FLASH_END,
                           &flag,
                           sizeof(flag),
                           &resp,
                           2000);
}

/******************************************************************************
 *  Baud Rate Switch
 ******************************************************************************/

br_result_t bootrom_change_baud(serial_handle_t *sh,
                                uint32_t old_baud,
                                uint32_t new_baud)
{
    br_change_baud_t cb;
    cb.new_baud = new_baud;
    cb.old_baud = old_baud;

    br_response_t resp;

    /* Tell ROM to switch */
    br_result_t r = br_pkt_send_cmd(sh,
                                    BR_CMD_CHANGE_BAUD,
                                    &cb,
                                    sizeof(cb),
                                    &resp,
                                    1000);
    if (r != BR_OK)
        return r;

    /* Reconfigure host serial port */
    serial_close(sh);
    sh = serial_open("/dev/ttyACM0", new_baud);  /* TODO: dynamic port */

    if (!sh)
        return BR_ERR_IO;

    /* Re-sync at new speed */
    return bootrom_sync(sh);
}

/******************************************************************************
 *  Reset
 ******************************************************************************/

void bootrom_reset(serial_handle_t *sh)
{
    serial_set_dtr(sh, 0);
    serial_set_rts(sh, 1);
    serial_set_rts(sh, 0);
}

