/*
 * fonts.h
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_FONTS_H_
#define SRC_BUFFER_FONTS_H_

#include <stdint.h>

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint32_t *data;
}FontDef;

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;
extern FontDef Font_32x55;
extern FontDef Font_32x80;

#endif /* SRC_BUFFER_FONTS_H_ */
