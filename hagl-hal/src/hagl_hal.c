/*

MIT License

Copyright (c) 2019-2021 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the SDL2 HAL for the HAGL graphics library:
https://github.com/tuupola/hagl_sdl2

SPDX-License-Identifier: MIT

*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <rgb565.h>
#include <bitmap.h>
#include <window.h>

#include <hagl_hal.h>

#include <string.h> 
#include <linux/fb.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include <errno.h>

static bitmap_t bb = {
    .width = DISPLAY_WIDTH,
    .height = DISPLAY_HEIGHT,
    .depth = DISPLAY_DEPTH,
};

int fb_data_size, fbfd, fb_width, fb_height, fb_bpp, fb_bytes;
char *fbdata;



void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    ((int *) fbdata )[(y0 * fb_width + x0)] = color;

    /* Tracking dirty window is not needed by HAGL. Below code is included */
    /* as an example if you want to iplement it in your HAL. */
}

void hagl_hal_put_pixels(uint32_t x0, uint32_t y0, uint32_t width, uint32_t height, uint32_t *colors){
    printf("hal writing pixels nn\n");
    for(int j = y0; j < height+y0; j++){
        for(int i = x0; i < width+x0; i++){
            ((int *) fbdata )[(j * fb_width + i)] = *(colors + height*(j-y0) + i - x0);
        }
    }
    

}

color_t hagl_hal_get_pixel(int16_t x0, int16_t y0)
{
    return *(color_t *) (bb.buffer + bb.pitch * y0 + (bb.depth / 8) * x0);
}
struct fb_var_screeninfo vinfo;
bitmap_t *hagl_hal_init(void)
{
    char fbdev[] = "/dev/graphics/fb0";
    fbfd = open (fbdev, O_RDWR);
    

    ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo);

    fb_width = vinfo.xres;
    fb_height = vinfo.yres;
    fb_bpp = vinfo.bits_per_pixel;
    vinfo.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;

    fb_bytes = fb_bpp / 8;

    printf("width %d height %d bytes-per-pixel %d\n", fb_width, fb_height, fb_bytes);

    fb_data_size = fb_width * fb_height * fb_bytes;

    fbdata = (char*) mmap (0, fb_data_size, 
        PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

    fprintf(stderr, "Value of errno: %d\n", errno);
    perror("Error printed by perror");

    printf("mapped ptr %p \n", fbdata);
    bitmap_init(&bb, fbdata);
    
   return &bb;
}

/*
 * Flushes the back buffer to the SDL2 window
 */
size_t hagl_hal_flush()
{
    
    if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo) < 0){
        printf("Failed to refresh\n");
    }
    
    return DISPLAY_WIDTH * DISPLAY_HEIGHT * DISPLAY_DEPTH / 2;
}

void hagl_hal_close(void)
{
    munmap (fbdata, fb_data_size);
    close (fbfd);
}

void hagl_hal_clear_screen(){
    memset (fbdata, 0, fb_data_size);
}

color_t hagl_hal_color(uint8_t r, uint8_t g, uint8_t b)
{
    color_t color = rgb565(r, g, b);
    return (color >> 8) | (color << 8);
}