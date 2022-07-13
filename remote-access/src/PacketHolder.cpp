#include <packet_parser.h>
#include <iostream>
#include <vector>
#include <PacketHolder.hpp>
#include <string.h>
using namespace std;

device_pkt_t *check_valid_packet(device_pkt_t *dev_pkt, bool *wrongType, int size)
{

    *wrongType = false;
    device_pkt_trailer_t *trailer = NULL;
    if (dev_pkt->type == display_draw)
    {
        display_draw_pkt_t *dsp_pkt = (display_draw_pkt_t *)dev_pkt->data;
        void *draw_pkt = dsp_pkt->draw_pkt;
        int offset = 0;

        switch (dsp_pkt->type)
        {
        case draw_pixel:
        {
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
            //printf("draw_pixel_pkt trailer %x \n", trailer->trailer);
            //cout << "draw_pixel_pkt\n";
        }
        break;

        case draw_pixels:
        {
            printf("check draw pixs -- \n");
            draw_pixels_pkt *pixels_pkt = (draw_pixels_pkt *)draw_pkt;
            offset = pixels_pkt->size;
            trailer = (device_pkt_trailer_t *)(((char *)pixels_pkt->pixels) + offset);
        }
        break;
        case draw_char:
        {
            offset = sizeof(draw_char_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
            printf("draw_char_pkt trailer %x \n", trailer->trailer);
            cout << "draw_char_pkt\n";
        }
        break;

        case draw_string:
        {
            printf("valid packet %d \n", offset);
            draw_char_str_pkt *str_pkt = ((draw_char_str_pkt *)draw_pkt);
            offset += str_pkt->len;
            printf("str offset %d \n", offset);
            trailer = (device_pkt_trailer_t *)(((char *)(str_pkt->data)) + offset);
        }
        break;

        case draw_line:
        {
            offset = sizeof(draw_line_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
        }
        break;

        case draw_rectangle:
        {
            offset = sizeof(draw_rectangle_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
        }
        break;

        case draw_triangle:
        {
            draw_triangle_pkt *tri_pkt = (draw_triangle_pkt *)draw_pkt;
            offset = sizeof(draw_triangle_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
        }
        break;

        case draw_circle:
        {
            draw_circle_pkt *circ_pkt = (draw_circle_pkt *)draw_pkt;
            offset = sizeof(draw_circle_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
        }

        break;

        case draw_ellipse:
        {
            draw_ellipse_pkt *ellps_pkt = (draw_ellipse_pkt *)draw_pkt;
            offset = sizeof(draw_ellipse_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
        }

        break;

        case draw_polygon:
        {
            draw_polygon_pkt *poly_pkt = (draw_polygon_pkt *)draw_pkt;
            offset = sizeof(draw_ellipse_pkt);
        }
        break;

        case draw_rounded_rectangle:
        {
            draw_rounded_rect_pkt *rrect_pkt = (draw_rounded_rect_pkt *)draw_pkt;
            offset = sizeof(draw_rounded_rect_pkt);
            trailer = (device_pkt_trailer_t *)(((char *)draw_pkt) + offset);
        }
        break;

        case draw_image:

            break;

        default:
            *wrongType = true;
            return 0;
            break;
        }
    }
    else if (dev_pkt->type == display_command)
    {
        display_command_pkt_t *cmd_pkt = (display_command_pkt_t *)dev_pkt->data;

        switch (cmd_pkt->type)
        {
        case clean_all_screen:
            trailer = (device_pkt_trailer_t *)(((char *)cmd_pkt) + sizeof(display_command_packet_type));
            printf("command packet | clean_all_screen \n ");
            break;

        default:
            *wrongType = true;
            return 0;
            break;
        }
    }

    if (trailer != NULL)
    {
        if ((((char *)trailer) - ((char *)dev_pkt)) > size)
        {
            return 0;
        }
        if (trailer->trailer != END_WORD)
        {
            return 0;
        }
        else
        {
            return (device_pkt_t *)(((char *)trailer) + sizeof(device_pkt_trailer_t));
        }
    }

    return 0;
}

PacketHolder::PacketHolder()
{
    status = waitsForStart;
}
//if this method successes than do not discard or override to given memory block as an argument
device_pkt_t *PacketHolder::getPacket(int &size)
{
    device_pkt_t *ret;
    if (complete_dev_pkts.size() > 0)
    {
        ret = (device_pkt_t *)complete_dev_pkts[0]->pkt;
        size = complete_dev_pkts[0]->size;
        complete_dev_pkts.erase(complete_dev_pkts.begin());
        return ret;
    }
    else
    {
        return NULL;
    }
}
device_pkt_t *PacketHolder::getPacket()
{
    device_pkt_t *ret;
    if (complete_dev_pkts.size() > 0)
    {
        ret = (device_pkt_t *)complete_dev_pkts[0]->pkt;
        complete_dev_pkts.erase(complete_dev_pkts.begin());
        return ret;
    }
    else
    {
        return NULL;
    }
}

bool PacketHolder::putPacket(char *data, int size)
{
    if((size <= 0)||(data == NULL)) return false;
    //cout << "analyzing packet" << endl;
    if (analyzePacket(data, size) > 0)
    {
        return true;
    }
    return true;
}

int PacketHolder::getCompletedPacketNum()
{
    return complete_dev_pkts.size();
}

int PacketHolder::analyzePacket(char *ptr, int size)
{
    bool wrongType = false;
    int numOfFoundPackets = 0;
    device_pkt_t *dev_pkt = NULL, *next_dev_pkt = NULL;
    if (size == 0)
        return -1;

    //printf("got packet(s)\n");
    for (int i = 0; i <= size - (int)sizeof(device_pkt_trailer_t); i++)
    {
        //printf("i %d  %d \n", i, size);
        if (i <= size - (int)sizeof(device_pkt_t))
        {
            if (((device_pkt_t *)(ptr + i))->magic == MAGIC_WORD)
            {
                printf("magic ok \n");
                if (((device_pkt_t *)(ptr + i))->start_word == START_WORD)
                {
                    printf("start ok \n");
                    dev_pkt = (device_pkt_t *)(ptr + i);
                    clearIncompletePackets();
                    if ((next_dev_pkt = check_valid_packet(dev_pkt, &wrongType, size)) != 0)
                    {
                        cout << "detected valid a dev_pkt head & trailer" << " size " << getCompletedPacketNum() << endl;
                        //complete_dev_pkts.push_back(dev_pkt);
                        addCompletePacket(dev_pkt, ((char *)next_dev_pkt) - ((char *)dev_pkt));
                        numOfFoundPackets++;
                        dev_pkt = next_dev_pkt;
                        status = waitsForStart;
                    }
                    else
                    {
                        if (!wrongType)
                        {
                            addIncompletePacket((char *)dev_pkt, size - i);
                            status = waitsForEnd;
                            return numOfFoundPackets; //encountered with header but memory does not contain a tailer
                                                      //so rest of packet must be part of previously encountered header
                        }
                        cout << "packet head correct but rest not \n";
                        printf("last pkt head addr %p\n", dev_pkt);
                    }
                }
            }
        }
        if (( ((device_pkt_trailer_t *)(ptr + i))->trailer == END_WORD) && (status == waitsForEnd))
        {
            int packedPktSize;
            device_pkt_t *packedPkt;
            //incomplete_dev_pkt.push_back((device_pkt_t*)ptr);
            addIncompletePacket(ptr, i + sizeof(device_pkt_trailer_t));
            packedPkt = packIncompletePacket(packedPktSize);
            addCompletePacket(packedPkt, packedPktSize);
            numOfFoundPackets++;
            status = waitsForStart;
        }
    }

    if ((status == waitsForEnd) && (incomplete_pkts.size() > 0))
    {
        addIncompletePacket(ptr, size);
    }

    return numOfFoundPackets;
}


void PacketHolder::addIncompletePacket(char *pkt, int size)
{
    char *pkt_ = new char[size];
    memcpy(pkt_, pkt, size);
    cout << "adding to incomplete packets \n";
    incomplete_pkts.push_back(new DataSizeBundle(pkt_, size));
}


void PacketHolder::addCompletePacket(device_pkt_t *pkt, int size)
{
    char *pkt_ = new char[size];
    memcpy(pkt_, pkt, size);
    cout << "adding to complete packets addr ws " << static_cast<void*>(pkt_) << " size " << size << endl;
    complete_dev_pkts.push_back(new DataSizeBundle((char *)pkt_, size));
}

void PacketHolder::clearIncompletePackets()
{
    incomplete_pkts.clear();
}

device_pkt_t *PacketHolder::packIncompletePacket(int &pktSize)
{
    int ipktsize = incomplete_pkts.size();
    int offset = 0, size = 0;
    for (int i = 0; i < ipktsize; i++)
    {
        size += incomplete_pkts[i]->size;
    }
    char *packedPacket = new char[size];
    pktSize = size;
    for (int i = 0; i < ipktsize; i++)
    {
        memcpy(packedPacket + offset, incomplete_pkts[i]->pkt, incomplete_pkts[i]->size);
        offset += incomplete_pkts[i]->size;
        delete incomplete_pkts[i]->pkt;
    }

    incomplete_pkts.clear();

    return (device_pkt_t *)packedPacket;
}

/*
int main(int argc, char** argv) {
    int size0, size1;
    int gsize1 = 0, gsize2 = 0, gsize3 = 0;
    wchar_t testWCHAR[] = L"0asdasdasdasdasbnetyjneyut6şrsgtjştrdfgtkjAll concertos for 1 violin by Bach. They are BWV 1041 and 1042: the well-known concertos. BWV1052, 1056 and 1064 are reconstructions. The latter one being for three violins. Soloists are among others Emmy Verhey and Rainer Kussmaul.ıkldtrjhytıoetyhıotehyrjjgaısjırşoıojşsgıjoşshhtsjsthrdfkjbvnhsdgfbhşksırhnobktrsıjtrsdnbşjfedwpıgşjduesrplcvujknelrouhk95uhıjgtreupjıştrwgfujwtpojgpo5y6jıutty5jmruyt78";
    device_pkt_t * pktClear = get_cmd_all_clear_pkt(&size0);
    device_pkt_t * pktStr = get_draw_string_pkt(0, 0, 0,&size1, testWCHAR, sizeof(testWCHAR));
    PacketHolder holder;


    // cout << "holder returns " << holder.putPacket((char*)pktClear, size0) << "\n";

    // cout << "holder num of complete packets" << holder.getCompletedPacketNum() << "\n";   
    
    // cout << "holder returns " << holder.putPacket((char*)pktStr, size1) << "\n";

    // cout << "holder num of complete packets" << holder.getCompletedPacketNum() << "\n";

    // cout << "size0 " << size0 << " size1 " << size1 << endl;


    // cout << "\n\n\n";

    // printWSize((char*) pktClear, size0);
    // cout << "\n\n\n";
    // printWSize((char*) pktStr, size1);
    // cout << "\n\n\n";
     
    // device_pkt_t * pkt1 =  holder.getPacket(gsize1);
    // cout << "getpacket returns addr = " << static_cast<void*>(pkt1) << " packed size " << gsize1 << std::endl;

    // device_pkt_t * pkt2 =  holder.getPacket(gsize2);
    // cout << "getpacket returns addr = " << static_cast<void*>(pkt2) << " packed size " << gsize2 << std::endl;

    // device_pkt_t * pkt3 =  holder.getPacket(gsize3);
    // cout << "getpacket returns addr = " << static_cast<void*>(pkt3) << " packed size " << gsize3 << std::endl;  

    // cout << "contents of pkt 1 " << endl;
    // printWSize((char*) pkt1, gsize1);

    // cout << "\n\n\n";
    // cout << "contents of pkt 2 " << endl;
    // printWSize((char*) pkt2, gsize2);
    // cout << "\n\n\n";

    


    cout << "holder num of complete packets" << holder.getCompletedPacketNum() << "\n";
    int padSize = 160;
    int paddedSize = size1+padSize;
    char *paddedPacket = new char[paddedSize];

    memcpy(paddedPacket + padSize, pktStr, size1);
    
    
    int block_size = 300;
    for(int i = 0; i < paddedSize; i+=block_size){
        holder.putPacket((char*)paddedPacket + i, block_size);
    }
    
    device_pkt_t * pkt1 =  holder.getPacket(gsize1);
    cout << "getpacket returns addr = " << static_cast<void*>(pkt1) << " packed size " << gsize1 << std::endl;

    cout << "contents of pkt 1 " << endl;
    printWSize((char*) pkt1, gsize1);

    cout << "\n\n\n";
    printWSize((char*) pktStr, size1);
    if(gsize1 == size1){
        cout << "\n\n\n "<<"yayyyyyy both are equal"<< "\n";
        cout << "memecomp " << memcmp(pkt1, pktStr, size1) << "\n";
    }
    


    return 0;
}
*/