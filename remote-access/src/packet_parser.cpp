#include <packet_parser.h>
#include <vector>
#include <iostream>
using namespace std;



device_pkt_t** get_accumulated_pkt_heads(char *ptr, int size, int *pkt_cnt, bool* anyFail, 
                                        device_pkt_t** lastPktHead, uint32_t* lastPktSize){
    vector<device_pkt_t*> *dev_pkt_heads = new vector<device_pkt_t*>;
    device_pkt_t *dev_pkt, *next_dev_pkt;
    *anyFail = false;
    bool wrongType;
    if(size == 0){
        *pkt_cnt = 0;
        return NULL;
    }
    //printf("got packet(s)\n");
    for(int i = 0; i < size - sizeof(device_pkt_t); i++){
        //printf("i %d  %d \n", i, size);
        if(((device_pkt_t*)(ptr + i))->magic == MAGIC_WORD){
            printf("magic ok \n");
            if(((device_pkt_t*)(ptr + i))->start_word == START_WORD){
                printf("start ok \n");
                dev_pkt = (device_pkt_t*)(ptr + i);
                if((next_dev_pkt = check_valid_packet(dev_pkt, &wrongType)) != 0){
                    cout << "detected valid a dev_pkt head & trailer \n";
                    dev_pkt_heads->push_back(dev_pkt);
                    *pkt_cnt = *pkt_cnt + 1;
                    dev_pkt = next_dev_pkt; 
                }else{
                    cout << "packet head correct but rest not \n";
                    printf("last pkt head addr %p\n", dev_pkt);
                    int sizeOfPrevPkts = ((char*)dev_pkt - (char*)ptr);
                    *anyFail = true;
                    *lastPktSize = size - sizeOfPrevPkts;
                    *lastPktHead = dev_pkt;
                    printf("size of prev pkts %d total %d lastpkt %d %p\n", 
                            sizeOfPrevPkts, size, size - sizeOfPrevPkts, lastPktHead);
                }
            }
        }
    }

    //cout<<"head cnt " << dev_pkt_heads->size() << "\n";
    device_pkt_t** heads = (device_pkt_t**) dev_pkt_heads->data();

    
    return heads;
}

device_pkt_t * check_valid_packet(device_pkt_t *dev_pkt, bool* wrongType){
    
    *wrongType = false;
    device_pkt_trailer_t *trailer = NULL;
    if(dev_pkt->type == display_draw){
        display_draw_pkt_t *dsp_pkt = (display_draw_pkt_t*) dev_pkt->data;
        void *draw_pkt = dsp_pkt->draw_pkt;
        int offset = 0;
        
        switch(dsp_pkt->type){
            case draw_pixel:
            {
                trailer = (device_pkt_trailer_t*)(((char *)draw_pkt) + offset);
                //printf("draw_pixel_pkt trailer %x \n", trailer->trailer);
                //cout << "draw_pixel_pkt\n";
            }
            break;

            case draw_pixels:
            {
                printf("check draw pixs \n");
                draw_pixels_pkt *pixels_pkt = (draw_pixels_pkt*) draw_pkt;
                offset = pixels_pkt->size;
                trailer = (device_pkt_trailer_t*)(((char *)pixels_pkt->pixels) + offset);
                printf("checking draw pixels pkt\n size %d trailer %x", offset, trailer->trailer);
            }
            break;
            case draw_char:
            {
                offset = sizeof(draw_char_pkt);
                trailer = (device_pkt_trailer_t*)(((char *)draw_pkt) + offset);
                printf("draw_char_pkt trailer %x \n", trailer->trailer);
                cout << "draw_char_pkt\n";
            }
            break;

            case  draw_string:
            {
                printf("valid packet %d \n", offset);
                draw_char_str_pkt* str_pkt = ((draw_char_str_pkt*)draw_pkt);
                offset += str_pkt->len;
                printf("str offset %d \n", offset);
                trailer = (device_pkt_trailer_t*)(((char*)(str_pkt->data)) + offset);
            }
            break;

            case  draw_line:
            {
                    offset = sizeof(draw_line_pkt);
                    trailer = (device_pkt_trailer_t*)(((char*)draw_pkt) + offset);
            }
            break;

            case  draw_rectangle:
            {
                    offset = sizeof(draw_rectangle_pkt);
                    trailer = (device_pkt_trailer_t*)(((char*)draw_pkt) + offset);
            }
            break;

            case  draw_triangle:
            {
                    draw_triangle_pkt *tri_pkt = (draw_triangle_pkt*) draw_pkt;
                    offset = sizeof(draw_triangle_pkt);
                    trailer = (device_pkt_trailer_t*)(((char*)draw_pkt) + offset);
            }
            break;

            case  draw_circle:
            {
                    draw_circle_pkt *circ_pkt = (draw_circle_pkt*) draw_pkt;
                    offset = sizeof(draw_circle_pkt);
                    trailer = (device_pkt_trailer_t*)(((char*)draw_pkt) + offset);
                
            }

            break;

            case  draw_ellipse:
            {
                    draw_ellipse_pkt *ellps_pkt = (draw_ellipse_pkt*) draw_pkt;
                    offset = sizeof(draw_ellipse_pkt);
                    trailer = (device_pkt_trailer_t*)(((char*)draw_pkt) + offset);

            }

            break;

            case  draw_polygon:
            {
                    draw_polygon_pkt *poly_pkt = (draw_polygon_pkt*) draw_pkt;
                    offset = sizeof(draw_ellipse_pkt);

            }       
            break;

            case  draw_rounded_rectangle:
            {
                    draw_rounded_rect_pkt *rrect_pkt = (draw_rounded_rect_pkt*) draw_pkt;
                    offset = sizeof(draw_rounded_rect_pkt);
                    trailer = (device_pkt_trailer_t*)(((char*)draw_pkt) + offset);

            }   
            break;

            case  draw_image:

            break;

            default:
                *wrongType = true;
                return 0;
            break;  
        }
        

    } else if(dev_pkt->type == display_command){
        display_command_pkt_t *cmd_pkt = (display_command_pkt_t*) dev_pkt->data;
        
        switch (cmd_pkt->type){
            case clean_all_screen:
                trailer = (device_pkt_trailer_t*) (((char*)cmd_pkt) + sizeof(display_command_packet_type));
                printf("command packet | clean_all_screen \n ");
            break;

            default:
                *wrongType = true;
                return 0;
            break;
        }
    }


    if(trailer != NULL){
        if(trailer->trailer != END_WORD){
            return 0;
        }else{
            return (device_pkt_t *) trailer + sizeof(device_pkt_trailer_t);
        }
    }

    return 0;


}

class PacketWrapper{
    public:
    char* raw_packet;
    uint32_t size;
    PacketWrapper(char *pkt, uint32_t size){

    }
    
};

/*
class packetParser{
    public:

    packetParser(){

    }

    void putData(char *ptr, int size){

    }

    private:
    vector<device_pkt_t*> complete_dev_pkt_heads;
    vector<device_pkt_t*> incomplete_dev_pkt;
    enum State{
        noIncompletePacket,
        incompletePacket
    };

    State incompletePacketStat = State::noIncompletePacket;




    int analyzePacket(char *ptr, int size){
        bool wrongType = false;
        device_pkt_t *dev_pkt, *next_dev_pkt;
        if(size == 0){
            return -1;
        }
        //printf("got packet(s)\n");
        for(int i = 0; i < size - sizeof(device_pkt_t); i++){
            //printf("i %d  %d \n", i, size);
            if(((device_pkt_t*)(ptr + i))->magic == MAGIC_WORD){
                printf("magic ok \n");
                if(((device_pkt_t*)(ptr + i))->start_word == START_WORD){
                    printf("start ok \n");
                    dev_pkt = (device_pkt_t*)(ptr + i);
                    incomplete_dev_pkt.clear();
                    if((next_dev_pkt = check_valid_packet(dev_pkt, &wrongType)) != 0){
                        cout << "detected valid a dev_pkt head & trailer \n";
                        complete_dev_pkt_heads.push_back(dev_pkt);
                        dev_pkt = next_dev_pkt; 
                    }else{
                        if(!wrongType){
                            incomplete_dev_pkt.push_back(dev_pkt);
                        }
                        cout << "packet head correct but rest not \n";
                        printf("last pkt head addr %p\n", dev_pkt);
                    }
                }
            }
            if(((device_pkt_trailer_t*)(ptr + i))->trailer == END_WORD){
                incomplete_dev_pkt.push_back(ptr);
            }
        }

    }
};
*/