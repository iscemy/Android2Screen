#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <hagl.h>
#include <font6x9.h>
#include <fcntl.h>
#include <wchar.h>
#include <string.h>


/*

MIT No Attribution

Copyright (c) 2019-2021 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/
#include <time.h>
#include <stdint.h>
#include <stdbool.h>


#include "hagl_hal.h"

#include "rgb565.h"
#include "rgb888.h"
#include "fps.h"

uint32_t flush_callback(uint32_t interval, void *param)
{
    hagl_flush();
    return interval;
}

uint32_t pps_callback(uint32_t interval, void *param)
{
    printf("Primitives per second: %f\n", *(float *)param);
    return interval;
}

void polygon_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x1 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y1 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x2 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y2 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x3 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y3 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x4 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y4 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};

    hagl_draw_polygon(5, vertices, colour);
}

void fill_polygon_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x1 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y1 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x2 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y2 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x3 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y3 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x4 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y4 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};

    hagl_fill_polygon(5, vertices, colour);
}

void circle_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    uint16_t r = (rand() % 40);
    int32_t colour = rand() % 0xffffffff;

    hagl_draw_circle(x0, y0, r, colour);
}

void fill_circle_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    uint16_t r = (rand() % 40);
    int32_t colour = rand() % 0xffffffff;

    hagl_fill_circle(x0, y0, r, colour);
}

void line_demo()
{
    int16_t x0 = (rand() % 600); /* -20 ... 340 */
    int16_t y0 = (rand() % 300); /* -20 ... 260 */
    int16_t x1 = (rand() % 600); /* -20 ... 340 */
    int16_t y1 = (rand() % 300); /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;
    hagl_draw_line(x0, y0, x1, y1, colour);
}

void rectangle_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x1 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y1 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x2 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y2 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x3 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y3 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x4 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y4 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;

    hagl_draw_rectangle(x0, y0, x1, y1, colour);
}

void fill_rectangle_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x1 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y1 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x2 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y2 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x3 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y3 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x4 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y4 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;

    hagl_fill_rectangle(x0, y0, x1, y1, colour);
}

void put_character_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;
    char ascii = rand() % 127;

    hagl_put_char(ascii, x0, y0, colour, font6x9, 3);
}

void put_text_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;

    hagl_put_text(L"YO! MTV raps.", x0, y0, colour, font6x9, 3);
}

void put_pixel_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;

    hagl_put_pixel(x0, y0, colour);

}

void triangle_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x1 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y1 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x2 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y2 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;

    hagl_draw_triangle(x0, y0, x1, y1, x2, y2, colour);
}

void fill_triangle_demo()
{
    int16_t x0 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y0 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x1 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y1 = (rand() % 600) - 20; /* -20 ... 260 */
    int16_t x2 = (rand() % 1024) - 20; /* -20 ... 340 */
    int16_t y2 = (rand() % 600) - 20; /* -20 ... 260 */
    int32_t colour = rand() % 0xffffffff;

    hagl_fill_triangle(x0, y0, x1, y1, x2, y2, colour);
}

void scale_blit_demo()
{
}

void rgb_demo()
{
    int32_t red =   rgb888(255, 0, 0);
    int32_t green = rgb888(0, 255, 0);
    int32_t blue =  rgb888(0, 0, 255);
    int16_t x0 = 0;
    int16_t x1 = DISPLAY_WIDTH / 3;
    int16_t x2 = 2 * x1;

    hagl_fill_rectangle(x0, 0, x1 - 1, DISPLAY_HEIGHT, red);
    hagl_fill_rectangle(x1, 0, x2 - 1, DISPLAY_HEIGHT, green);
    hagl_fill_rectangle(x2, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, blue);
}




int main(int argc, char **argv){
    char read_buffer[64];
    wchar_t wchar_buffer[64];
    int read_size;
    unsigned int color_black = hagl_color(0,0,0);

 

    void (*demo[13]) ();

    //demo[0] = triangle_demo;
    //demo[4] = triangle_demo;
    //demo[5] = fill_rectangle_demo;
    //demo[6] = rectangle_demo;
    //demo[7] = line_demo;
    //demo[8] = circle_demo;
    //demo[9] = fill_circle_demo;
    //demo[10] = polygon_demo;
    //demo[11] = fill_polygon_demo;
    //demo[12] = put_text_demo;
    demo[0] = rgb_demo;
    demo[1] = put_character_demo;
    demo[2] = put_pixel_demo;
    demo[3] = fill_triangle_demo;
    demo[4] = triangle_demo;
    demo[5] = fill_rectangle_demo;
    demo[6] = rectangle_demo;
    demo[7] = line_demo;
    demo[8] = circle_demo;
    demo[9] = fill_circle_demo;
    demo[10] = polygon_demo;
    demo[11] = fill_polygon_demo;
    demo[12] = put_text_demo;



    hagl_init();
    
    
    //for(int i = 0; i < 10; i++){
    //    swprintf(wchar_buffer, 60, L"Hellow: %d", i);
    //    hagl_put_text(wchar_buffer, 0, i*30, hagl_color(0,0,0), font6x9, 3);
    //    hagl_flush();
    //}

    int current_demo = 0, demo_cnt = 0, lineNumber = 0;;
    printf("starting demos \n");

    while(1){
        
        
        //if(read_size > 0){
        //    mbstowcs(wchar_buffer, read_buffer, read_size-1);
        //    wprintf(L"test %ls\n", wchar_buffer);
        //    hagl_put_text(wchar_buffer, 0 , 30 + lineNumber*30, color_black, font6x9, 3);

        //    memset(wchar_buffer, L' ', sizeof(wchar_t)*64);
        //    if(lineNumber > 15){
        //        hagl_clear_screen();
        //        lineNumber = 0;
        //    }
        //    lineNumber++;
        //}

        demo[current_demo%sizeof(demo)]();
        
        usleep(10*1000);
        demo_cnt++;
        if(demo_cnt > 100){
            demo_cnt = 0;
            current_demo = (current_demo+1)%(sizeof(demo) - 1);
            hagl_clear_screen();
            printf("to next demo %d \n", current_demo);
        }
        
        
    }

    

    sleep(2);
//
    
//
    
//
    err0:

    return 0;
}