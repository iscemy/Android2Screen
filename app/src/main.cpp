#define SSIZE_MAX 500000
#include <iostream>
#include <remote_access.h>
#include <string>
#include <unistd.h>
#include <packet_parser.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <cerrno>
#include <cstring>
#include <vector>
#include <hagl.h>
#include <PacketHolder.hpp>
using namespace std;

class CircBuffer{
    char *bufferStart;
    char *helperBuffer;
    int maxSize;
    int head, tail;
    public:

    CircBuffer(int size){
        this->maxSize = size;
        bufferStart = new char[size];
        helperBuffer = new char[size];
        head = 0;
        tail = 0;
    }
    int putSome(char *buffer, int size){
        if(size <= 0) return -1;
        if(head + size < maxSize){
            memcpy(bufferStart + head, buffer, size);
            head += size;
        }else{
            memcpy(bufferStart + head, buffer, maxSize-head);
            memcpy(bufferStart, buffer + maxSize - head, size - maxSize + head);
            head = size - maxSize + head;
        }
        return 0;
    }

    char *getAll(int* size){
        *size = 0;

        char * retPtr;
        if(head == tail){
            return NULL;
        }else if(head > tail){
            *size = head - tail;
            retPtr = bufferStart + tail;
            tail = head;
            return retPtr;
        }else if(head < tail){
            memcpy(helperBuffer, bufferStart + tail, maxSize - tail);
            memcpy(helperBuffer + maxSize - tail, bufferStart, head);
            *size = maxSize - tail + head;
            tail = head;
            return helperBuffer;
        }
    }

    
};

int main(int argc, char **argv){

    cout<<"Hellow Cpp and welc\n";
    char modes[] = "adb,acm";
    //set_usb_modes(modes, 7);


    hagl_init();
    
    char buffer[4095*4]; // TODO: IMPLEMENT IN A SHRINKABLE GROWABLE data structure
    int fd = open("/dev/ttyGS0", O_RDWR);
    if(fd < 0 ){
        printf("cant open serial port \n");
        return -1;
    }
    // sleep(2);
    // sleep(2);
    // sleep(2);

    struct termios tty;
    cout<<"1 \n";
    if(tcgetattr(fd, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag |= PARENB;  // Set parity bit, enabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CSTOPB;  // Set 
    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    tty.c_cflag |= CS5; // 5 bits per byte
    tty.c_cflag |= CS6; // 6 bits per byte
    tty.c_cflag |= CS7; // 7 bits per byte
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CRTSCTS;  // Enable RT
    tty.c_cflag |= CREAD | CLOCAL; // Turn 
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disa
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;
    cfsetispeed(&tty, B1152000);
    cfsetospeed(&tty, B1152000);
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    int readCnt, size = 0;
    device_pkt_t** d;
    int c = 0;






    PacketHolder packetHolder;
    device_pkt_t *pkt;
    size = 0;
    char *bufferForTest= new char[5000], *t;
    while(1){
        
        usleep(100);

        //cout << "reading from usb " << endl;
        readCnt = read(fd, bufferForTest, 5000);
        //  cout << "read return read cnt " << readCnt << endl;
        if(packetHolder.putPacket(bufferForTest, readCnt)){
            cout << "packet putted to packet holder "<< "read cnt " << readCnt << " " << static_cast<void*>(bufferForTest) << " size " << packetHolder.getCompletedPacketNum() << " \n";
        }
        if(packetHolder.getCompletedPacketNum() > 0){
            if ((pkt = packetHolder.getPacket(size)) != 0){
                cout << "got packet " << size << " addr " << static_cast<void*>(pkt) << " size " << packetHolder.getCompletedPacketNum()  << endl;
                process_packet(pkt);
                delete pkt;
                cout << "deleted from heap" << endl;
            }
        }
        // circBuffer.putSome(buffer, readCnt);
        // if(readCnt > 0){
        //     printf("read some %d buffer offset %d\n", readCnt, bufferOffset);
        // }

        // circOut = circBuffer.getAll(&circBufferSize);
        // if(circOut != NULL){
        //     d = get_accumulated_pkt_heads(circOut, circBufferSize, &size, &anyFail, &lastPktHead, &lastPktSize);
        // }
        
        // if(anyFail){
        //     printf("\nlast packet is not complete\n");
        //     if( (char*) lastPktHead != buffer){
        //         printf("packet is not first pkt in buffer %p %p\n", lastPktHead, buffer);
        //         memcpy(buffer, lastPktHead, lastPktSize);
        //         printf("writing last packet to head of buffer\n");
        //     }
        //     bufferOffset = lastPktSize;
        // }else{
        //     bufferOffset = 0;
        // }

        
        // if(size > 0){
        //     printf("accumulated pkt cnts %d\n", size);
        // }
        // for(int i = 0; i < size; i++){
        //     process_packet(d[i]);

        // }
        // size = 0;
        // free(d);
    }


    
    
    
    cout<<"accumulated_pkt_cnts " << size << "\n";

    return 0;
}
