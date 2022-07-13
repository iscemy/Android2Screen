#ifndef WRAPPER
#define WRAPPER

#define MAGIC_WORD 0x54CBA13491AEF6FF
#define START_WORD 0xDEADBEEF
#define END_WORD 0xDEAF135F
#include <wchar.h>
#include <stddef.h>
#include <stdint.h>
typedef enum {
    display_draw,
    display_command,
    touch
}packet_type;
typedef enum {
    draw_pixel = 0,
    draw_pixels,
    draw_char,
    draw_string,
    draw_line,
    draw_rectangle,
    draw_triangle,
    draw_circle,
    draw_ellipse,
    draw_polygon,
    draw_rounded_rectangle,
    draw_image,
}display_draw_packet_type;

typedef enum {
    clean_all_screen
}display_command_packet_type;

typedef struct device_pkt_trailer_t
{
    uint32_t trailer;
}device_pkt_trailer_t;


typedef struct device_pkt_t
{
    uint64_t magic;
    uint32_t start_word;
    packet_type type;   
    char data[0];
}device_pkt_t;

typedef struct 
{
    uint32_t x0;
    uint32_t y0;
    char filled;
    int32_t color; //r8g8b8
    display_draw_packet_type type;
    char draw_pkt[0];
}display_draw_pkt_t;

typedef struct 
{
    display_command_packet_type type;
    char _cmd_trailer[0];
    
}display_command_pkt_t;


typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t size;
    uint32_t pixels[0];
}draw_pixels_pkt;

typedef struct  
{
    uint32_t len; //
    int16_t points[0];
}draw_polygon_pkt;

typedef struct  
{
    uint32_t widht; uint32_t height;
    uint32_t byteppp;
    uint32_t points[0];
}draw_image_pkt;

typedef struct  
{
    uint32_t x1, y1;
    uint32_t r;
}draw_rounded_rect_pkt;


typedef struct  
{
    uint32_t r;
}draw_circle_pkt;

typedef struct  
{
    uint32_t a; uint32_t b;
}draw_ellipse_pkt;

typedef struct  
{
    uint32_t scale;
    wchar_t data;
}draw_char_pkt;

typedef struct  
{
    uint32_t scale;
    uint32_t len;
    wchar_t data[0];
}draw_char_str_pkt;

typedef struct  
{
    uint32_t x1; uint32_t y1;
}draw_line_pkt;

typedef struct  
{
    uint32_t x1; uint32_t y1;
}draw_rectangle_pkt;

typedef struct  
{
    uint32_t x1; uint32_t y1;
    uint32_t x2; uint32_t y2;
}draw_triangle_pkt;


#endif
