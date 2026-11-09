/******************************************************************************
 * File:        image.h
 * Description: Public interface for parsing firmware image@offset specifications.
 * Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
 * Date:        2026110601
 * Version:     v1.2.0
 ******************************************************************************/
// 2026110601 (MFC/PP/AGY) Upgraded to canonical Crusan & Co. 5-Field SOP Header standard.

#ifndef AXFLASH_IMAGE_H
#define AXFLASH_IMAGE_H

#include "flash.h"

void image_parse(flash_image_t *img, const char *spec);

#endif /* AXFLASH_IMAGE_H */
