//
//  ycbcr_to_rgb.c
//  mjpeg423app
//
//  Created by Rodolfo Pellizzoni on 12/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "mjpeg423_decoder.h"
#include "../common/util.h"
#include "system.h"

#ifndef NULL_COLORCONV

//normalize result between 0 and 255
//this is required to handle precision errors that might cause the obtained color to fall out of range
#define NORMALIZE_RGB(x) (temp = (x), ( (temp < 0) ? 0 : ( temp = (temp >> 14), (temp > 255) ? 255 : temp  ) ) )

//fixed point version of the 4:4:4 miniblock conversion
//values are first scaled by 14 bits, then summed, then rescaled back by 14 bits
//the result fits in 24 bits when using two's complement
//(1 bit sign, 14 bits scaling, 8 bit original sample + 1 additional bit since the
//result can overflow due to imprecisions added in the quantizaton)
void ycbcr_to_rgb(int h, int w, uint32_t w_size, pcolor_block_t Y, pcolor_block_t Cb, pcolor_block_t Cr, rgb_pixel_t* rgbblock)
{
    int index;
    for (int y = 0; y < 8; y++){
        index = (h+y) * w_size + w;
        for(int x = 0; x < 8; x++){
        	uint32_t Cbr = Cr[y][x] << 8 | Cb[y][x];
        	rgb_pixel_t pixel;
        	uint32_t rawPixel;
        	rawPixel = ALT_CI_YCBCR_TO_RGB_INST(Y[y][x],Cbr);
            pixel.alpha = 0;
            pixel.blue = rawPixel;
            pixel.green = rawPixel >> 8;
            pixel.red = rawPixel >> 16;
            rgbblock[index] = pixel;
            index++;
        }
    }
}

#else

//null implementation
void ycbcr_to_rgb(int h, int w, uint32_t w_size, pcolor_block_t Y, pcolor_block_t Cb, pcolor_block_t Cr, rgb_pixel_t* rgbblock)
{
    int index;
    for (int y = 0; y < 8; y++){
        index = (h+y) * w_size + w;
        for(int x = 0; x < 8; x++){
            rgb_pixel_t pixel;
            pixel.alpha = 0;
            pixel.red = Cr[y][x];
            pixel.green = Y[y][x];
            pixel.blue = Cb[y][x];
            rgbblock[index] = pixel;
            index++;
        }
    }
}

#endif
