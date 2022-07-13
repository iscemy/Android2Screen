
#include <wrapper.h>
#include <stdio.h>
#include <hagl.h>
#include <font6x9.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>
int parse_and_run_dsp_draw_packet(display_draw_pkt_t *dsp_pkt){
    int x0 = dsp_pkt->x0;
    int y0 = dsp_pkt->y0;

    bool filled = (dsp_pkt != 0x00)? 1:0;
    void *draw_pkt = dsp_pkt->draw_pkt;
    int color = dsp_pkt->color;
    switch(dsp_pkt->type){  
        case draw_pixel:
            hagl_put_pixel(x0, y0, dsp_pkt->color);
            //printf("put pixel x0:%d y0:%d\n", x0, y0);
        break;

        case draw_pixels:
        {
            printf("drawing pixels\n");
            draw_pixels_pkt *pixels_pkt = (draw_pixels_pkt*) draw_pkt;
            printf("info x0:%u  w:%u  y0:%u  h:%u\n", x0, pixels_pkt->width, y0, pixels_pkt->height);
            if(hagl_put_pixels(x0, y0, pixels_pkt->width, pixels_pkt->height, pixels_pkt->pixels) != 0){
                
            }

                
        }
        break;
        case draw_char:
            {
            draw_char_pkt *chr_pkt = (draw_char_pkt*) draw_pkt;
            hagl_put_char(chr_pkt->data, x0, y0 , color, 
                        font6x9, chr_pkt->scale);
            }
        break;

        case  draw_string:
            {
            //TODO: CONTROL STRING
            
            draw_char_str_pkt *str_pkt = (draw_char_str_pkt*) draw_pkt;
            
            printf("\n\n draw_string %d \n", (str_pkt->len));
            wchar_t local_str[] = L"test";


            for(int i = 0; i < str_pkt->len; i++){
                printf(" %x ", ((char*) str_pkt->data)[i] & 0xff);
                //local_str[i] = 
            } 
            int __last_element = (str_pkt->len) / 2;
            
            if((str_pkt->data[str_pkt->len-2] == 0x00) && (str_pkt->data[str_pkt->len-1] == 0x00) ){
                printf("writing to screen \n");
                hagl_put_text(L"Whaaa", 500, 500 , color, 
                            font6x9, 3);
                hagl_put_text_c(str_pkt->data, 200, 200 , color, 
                            font6x9, 3);
            }else{ 
                printf("\n\ndatas lenght and lenght defined by pkt not equal %d %d \n",
                         wcslen(str_pkt->data), (str_pkt->len) / 2 - 1);
            }
            }
        break;

        case  draw_line:
            {
                draw_line_pkt *line_pkt = (draw_line_pkt*) draw_pkt; 
                hagl_draw_line(x0, y0, line_pkt->x1, line_pkt->y1, color);
            }
        break;

        case  draw_rectangle:
            {
                draw_rectangle_pkt *rect_pkt = (draw_rectangle_pkt*) draw_pkt; 
                if(filled){
                    hagl_fill_rectangle(x0, y0, rect_pkt->x1, rect_pkt->y1, color);
                }else{
                    hagl_draw_rectangle(x0, y0, rect_pkt->x1, rect_pkt->y1, color);
                }

            }
        break;

        case  draw_triangle:
            {
                draw_triangle_pkt *tri_pkt = (draw_triangle_pkt*) draw_pkt;
                if(filled){
                    hagl_draw_triangle(x0, y0, tri_pkt->x1, tri_pkt->y1, 
                                        tri_pkt->x2, tri_pkt->y2, color);
                }else{
                    hagl_fill_triangle(x0, y0, tri_pkt->x1, tri_pkt->y1, 
                                        tri_pkt->x2, tri_pkt->y2, color);
                }
            }
        break;

        case  draw_circle:
            {
                draw_circle_pkt *circ_pkt = (draw_circle_pkt*) draw_pkt;
                if(filled)
                    hagl_draw_circle(x0, y0, circ_pkt->r, color);
                else
                    hagl_fill_circle(x0, y0, circ_pkt->r, color);               
            }

        break;

        case  draw_ellipse:
            {
                draw_ellipse_pkt *ellps_pkt = (draw_ellipse_pkt*) draw_pkt;
                if(filled)
                    hagl_draw_ellipse(x0, y0, ellps_pkt->a, ellps_pkt->b, color);
                else
                    hagl_fill_ellipse(x0, y0, ellps_pkt->a, ellps_pkt->b, color);
            }

        break;

        case  draw_polygon:
            {
                draw_polygon_pkt *poly_pkt = (draw_polygon_pkt*) draw_pkt;
                if(filled)
                    hagl_fill_polygon(poly_pkt->len, poly_pkt->points, color);
                else
                    hagl_draw_polygon(poly_pkt->len, poly_pkt->points, color);
            }       
        break;

        case  draw_rounded_rectangle:
            {
                draw_rounded_rect_pkt *rrect_pkt = (draw_rounded_rect_pkt*) draw_pkt;
                if(filled)
                    hagl_fill_rounded_rectangle(x0, y0, rrect_pkt->x1, 
                        rrect_pkt->y1, rrect_pkt->r, color);
                else
                    hagl_draw_rounded_rectangle(x0, y0, rrect_pkt->x1, 
                        rrect_pkt->y1, rrect_pkt->r, color);
            }   
        break;

        case  draw_image:

        break;


        default:
            printf("undefined dispaly pkt ");
        break;  
    }

    return 0;


}

int parse_and_run_dsp_command_packet(display_command_pkt_t *cmd_pkt){
    switch (cmd_pkt->type){

        case clean_all_screen:
           //printf("run command packet | clean_all_screen \n ");
           hagl_clear_screen();
        break;

        default:
            return 0;
        break;
    }
}

int process_packet(device_pkt_t *pkt){
    if((pkt->magic == MAGIC_WORD) && (pkt->start_word == START_WORD)){

        if(pkt->type == display_draw) {
            printf("packet type display\n");
            parse_and_run_dsp_draw_packet((display_draw_pkt_t*) pkt->data);
        } else if (pkt->type == touch) {
            printf("packet type touch\n");
        } else if (pkt->type == display_command){
            printf("packet type command\n");
            parse_and_run_dsp_command_packet((display_command_pkt_t*) pkt->data);
        }else{
            printf("incorrect type \n");
        }
    }else{
        printf("incorrect packet \n");
    }
    return 0;
}