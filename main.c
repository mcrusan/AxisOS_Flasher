/******************************************************************************
 * File:        main.c
 * Description: Entry point and high-level CLI orchestration for the Crusan & Co. AxisOS ESP32 flasher (axflash).
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serial.h"
#include "bootrom.h"
#include "flash.h"
#include "image.h"

/******************************************************************************
 *  Local Prototypes
 ******************************************************************************/

static void usage(void);
// static void progress_printer(uint32_t done, uint32_t total);

/******************************************************************************
 *  Local Functions
 ******************************************************************************/

static void usage(void) {
    printf("axflash - AxisOS ESP32 Flasher\n");
    printf("Usage:\n");
    printf("  axflash --port <device> --app <file@addr>\n");
    printf("         [--bootloader <file@addr>] [--partition <file@addr>]\n");
}

// static void progress_printer(uint32_t done, uint32_t total)
// {
//    float pct = (float)done * 100.0f / (float)total;
//    printf("\rFlashing: %.1f%%", pct);
//    fflush(stdout);
// }

/******************************************************************************
 *  Entry Point
 ******************************************************************************/

int main(int argc, char **argv) {
    const char *port = NULL;

    flash_image_t bootloader = {0};
    flash_image_t partition  = {0};
    flash_image_t app        = {0};

    /**************************************************************************
     *  Argument Parsing
     **************************************************************************/
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--port") && i + 1 < argc) {
            port = argv[++i];
        } else if (!strcmp(argv[i], "--bootloader") && i + 1 < argc) {
            image_parse(&bootloader, argv[++i]);
        } else if (!strcmp(argv[i], "--partition") && i + 1 < argc) {
            image_parse(&partition, argv[++i]);
        } else if (!strcmp(argv[i], "--app") && i + 1 < argc) {
            image_parse(&app, argv[++i]);
        }
    }

    if (!port || !app.file) {
        usage();
        return 1;
    }

    /**************************************************************************
     *  Serial Open
     **************************************************************************/
    serial_handle_t *sh = serial_open(port, 115200);
    if (!sh) {
        fprintf(stderr, "ERROR: Failed to open port %s\n", port);
        return 1;
    }

    /* Enter bootloader mode (GPIO0 low + EN pulse) */
    serial_enter_bootloader(sh);

    /**************************************************************************
     *  Bootloader Sync
     **************************************************************************/
    if (!bootrom_sync(sh)) {
        fprintf(stderr, "ERROR: Failed to sync with bootloader\n");
        serial_close(sh);
        return 1;
    }

    /**************************************************************************
     *  Flash Bootloader (Optional)
     **************************************************************************/
    if (bootloader.file) {
        printf("Flashing bootloader...\n");
        if (!flash_write(sh, &bootloader)) {
            fprintf(stderr, "ERROR: Bootloader flash failed\n");
            serial_close(sh);
            return 1;
        }
    }

    /**************************************************************************
     *  Flash Partition Table (Optional)
     **************************************************************************/
    if (partition.file) {
        printf("Flashing partition table...\n");
        if (!flash_write(sh, &partition)) {
            fprintf(stderr, "ERROR: Partition flash failed\n");
            serial_close(sh);
            return 1;
        }
    }

    /**************************************************************************
     *  Flash App Image (Required)
     **************************************************************************/
    printf("Flashing app...\n");
    if (!flash_write(sh, &app)) {
        fprintf(stderr, "ERROR: App flash failed\n");
        serial_close(sh);
        return 1;
    }

    /**************************************************************************
     *  Reset and Exit
     **************************************************************************/

    /* Tell ROM loader to jump to the flashed app */
    bootrom_reset_to_run(sh);

    /* Optional but recommended: physical EN pulse */
    serial_reset_pulse(sh);

    serial_close(sh);

    printf("Done.\n");
    return 0;
}
