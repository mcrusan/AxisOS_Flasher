/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher)
 *  File    : serial.h
 *  Author  : Marc / AxisOS
 *  Purpose : Public interface for serial port abstraction layer.
 ******************************************************************************/

#ifndef AXFLASH_SERIAL_H
#define AXFLASH_SERIAL_H

#include <stddef.h>

#if defined(_WIN32)
    typedef struct {
        void *handle;
    } serial_handle_t;
#else
    typedef struct {
        int fd;
    } serial_handle_t;
#endif

serial_handle_t *serial_open(const char *port, int baud);
void serial_close(serial_handle_t *sh);

int serial_write(serial_handle_t *sh, const void *data, size_t len);
int serial_read(serial_handle_t *sh, void *data, size_t len);

void serial_set_dtr(serial_handle_t *sh, int level);
void serial_set_rts(serial_handle_t *sh, int level);

int serial_reset_pulse(serial_handle_t *sh);
int serial_enter_bootloader(serial_handle_t *sh);


#endif /* AXFLASH_SERIAL_H */
