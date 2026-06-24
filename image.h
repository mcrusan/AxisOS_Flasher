/******************************************************************************
 *  Project : AxisOS - axflash (ESP32 Flasher)
 *  File    : image.h
 *  Author  : Marc / AxisOS
 *  Purpose : Public interface for parsing image specifications.
 ******************************************************************************/

#ifndef AXFLASH_IMAGE_H
#define AXFLASH_IMAGE_H

#include "flash.h"

void image_parse(flash_image_t *img, const char *spec);

#endif /* AXFLASH_IMAGE_H */
