
#include <vector>





class PacketHolder{

    public:
    PacketHolder();
    device_pkt_t *getPacket(int &size);
    device_pkt_t *getPacket();
    bool putPacket(char * data, int size);
    int getCompletedPacketNum();

    private:
    int analyzePacket(char *ptr, int size);

    void addIncompletePacket(char* pkt);
    void addIncompletePacket(char* pkt, int size);


    void addCompletePacket(device_pkt_t * pkt, int size);

    void clearIncompletePackets();
    device_pkt_t* packIncompletePacket(int &pktSize);

    enum PacketHolderStatus{
        hasCompletePacket,
        waitsForEnd,
        waitsForStart
    };

    struct DataSizeBundle{
        char *pkt;
        uint32_t size;
        DataSizeBundle(char *pkt, int size){
            this->pkt = pkt;
            this->size = size;
        }
        ~DataSizeBundle(){
            //delete pkt;
        }
    };


    PacketHolderStatus status;
    std::vector<DataSizeBundle*> incomplete_pkts;
    std::vector<DataSizeBundle*> complete_dev_pkts;


};