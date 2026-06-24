/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher)
 *  File    : image.c
 *  Author  : Marc / AxisOS
 *  Purpose : Parses file@address syntax and populates flash_image_t structures.
 ******************************************************************************/

#define _POSIX_C_SOURCE 200809L

#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 *  Parse "file@addr"
 ******************************************************************************/

void image_parse(flash_image_t *img, const char *spec)
{
    if (!img || !spec)
        return;

    char *copy = strdup(spec);
    char *at = strchr(copy, '@');

    if (at) {
        *at = '\0';
        img->file = strdup(copy);
        img->addr = strtoul(at + 1, NULL, 0);
    } else {
        img->file = strdup(copy);
        img->addr = 0;
    }

    free(copy);
}
