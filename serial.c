/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher)
 *  File    : serial.c
 *  Author  : Marc / AxisOS
 *  Purpose : Cross-platform serial port abstraction for the AxisOS flasher.
 *            Provides open/close/read/write and DTR/RTS control. All low-level
 *            serial operations are isolated here to keep the rest of the
 *            flasher clean and portable.
 ******************************************************************************/
#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 200809L

#include "serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>



/******************************************************************************
 *  Platform Includes
 ******************************************************************************/

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <termios.h>
    #include <unistd.h>
    #include <linux/serial.h>
#endif

/******************************************************************************
 *  Serial Open
 ******************************************************************************/

serial_handle_t *serial_open(const char *port, int baud)
{
    serial_handle_t *sh = calloc(1, sizeof(serial_handle_t));
    if (!sh)
        return NULL;

#if defined(_WIN32)

    // Windows implementation placeholder
    // (Real implementation will use CreateFileA, SetCommState, etc.)
    (void)port;
    (void)baud;

#else

    sh->fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (sh->fd < 0) {
        free(sh);
        return NULL;
    }

    struct termios t;
    memset(&t, 0, sizeof(t));

    cfmakeraw(&t);
    cfsetspeed(&t, baud);

    t.c_cflag |= (CLOCAL | CREAD);

    tcsetattr(sh->fd, TCSANOW, &t);

#endif

    return sh;
}

/******************************************************************************
 *  Serial Close
 ******************************************************************************/

void serial_close(serial_handle_t *sh)
{
    if (!sh)
        return;

#if defined(_WIN32)
    // Windows CloseHandle placeholder
#else
    if (sh->fd >= 0)
        close(sh->fd);
#endif

    free(sh);
}

/******************************************************************************
 *  Serial Write
 ******************************************************************************/

int serial_write(serial_handle_t *sh, const void *data, size_t len)
{
#if defined(_WIN32)
    // Windows WriteFile placeholder
    (void)sh; (void)data; (void)len;
    return -1;
#else
    return write(sh->fd, data, len);
#endif
}

/******************************************************************************
 *  Serial Read
 ******************************************************************************/

int serial_read(serial_handle_t *sh, void *data, size_t len)
{
#if defined(_WIN32)
    // Windows ReadFile placeholder
    (void)sh; (void)data; (void)len;
    return -1;
#else
    return read(sh->fd, data, len);
#endif
}

/******************************************************************************
 *  Serial DTR/RTS Control
 ******************************************************************************/

void serial_set_dtr(serial_handle_t *sh, int level)
{
#if defined(_WIN32)
    // Windows EscapeCommFunction placeholder
    (void)sh; (void)level;
#else
    // Linux/macOS: TIOCM_DTR
    int flags;
    ioctl(sh->fd, TIOCMGET, &flags);
    if (level)
        flags |= TIOCM_DTR;
    else
        flags &= ~TIOCM_DTR;
    ioctl(sh->fd, TIOCMSET, &flags);
#endif
}

void serial_set_rts(serial_handle_t *sh, int level)
{
#if defined(_WIN32)
    // Windows EscapeCommFunction placeholder
    (void)sh; (void)level;
#else
    // Linux/macOS: TIOCM_RTS
    int flags;
    ioctl(sh->fd, TIOCMGET, &flags);
    if (level)
        flags |= TIOCM_RTS;
    else
        flags &= ~TIOCM_RTS;
    ioctl(sh->fd, TIOCMSET, &flags);
#endif
}
/******************************************************************************
 *  Enter Bootloader Mode
 *  GPIO0 low + EN pulse → forces ESP32 into ROM bootloader.
 ******************************************************************************/
int serial_enter_bootloader(serial_handle_t *sh)
{
    serial_set_dtr(sh, 1);  /* GPIO0 low  */
    serial_set_rts(sh, 1);  /* EN low     */
    serial_set_rts(sh, 0);  /* EN high    */
    return 1;
}

/******************************************************************************
 *  Reset Pulse (Run Mode)
 *  GPIO0 high + EN pulse → resets ESP32 into normal application mode.
 ******************************************************************************/
int serial_reset_pulse(serial_handle_t *sh)
{
    serial_set_dtr(sh, 0);  /* GPIO0 high */
    serial_set_rts(sh, 1);  /* EN low     */
    serial_set_rts(sh, 0);  /* EN high    */
    return 1;
}

