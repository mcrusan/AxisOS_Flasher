/******************************************************************************
 * File:        image.c
 * Description: Parses file@address CLI syntax and populates flash_image_t structures.
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

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
