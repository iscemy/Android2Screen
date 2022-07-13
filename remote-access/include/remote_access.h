extern "C" {
#include<wrapper.h>
void set_usb_gadget_state(bool state);
void set_usb_modes(char *modes, int size);
int process_packet(device_pkt_t *pkt);
}