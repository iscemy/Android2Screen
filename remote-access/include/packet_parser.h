#include <wrapper.h>
device_pkt_t** get_accumulated_pkt_heads(char *ptr, int size, int *pkt_cnt, bool* anyFail, 
                                        device_pkt_t** lastPktHead, uint32_t* lastPktSize);
device_pkt_t * check_valid_packet(device_pkt_t *dev_pkt, bool *wrongType);


