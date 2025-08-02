#include <capture.h>
#include <rit.h>
#include <pcap.h>
#include <string.h>
#include <sys/types.h>

volatile char packet_list[MAX_PACKETS][MAX_PACKET_STR] = {0};
volatile int packet_count = 0;
volatile int packet_head = 0;

volatile int capture_pause = 0;

void* capture_thread(void* arg)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live("enp7s0", 1518, 1, 100, errbuf);
    if (!handle) {
        snprintf((char*)packet_list[0], MAX_PACKET_STR, "pcap_open_live failed: %s", errbuf);
        packet_count = 1;
        return NULL;
    }

    while (running) {
        struct pcap_pkthdr *header;
        const __u_char *data;
        int res = pcap_next_ex(handle, &header, &data);
        
        if (res == 1){
            char src_mac[18], dst_mac[18], src_ip[16] = "", protocol = 0;
            snprintf(dst_mac, sizeof(dst_mac), "%02X:%02X:%02X:%02X:%02X:%02X",
                data[0], data[1], data[2], data[3], data[4], data[5]);
            snprintf(src_mac, sizeof(src_mac), "%02X:%02X:%02X:%02X:%02X:%02X",
                data[6], data[7], data[8], data[9], data[10], data[11]);

            if (data[12] == 0x08 && data[13] == 0x00 && header->len >= 26) {
                snprintf(src_ip, sizeof(src_ip), "(%u.%u.%u.%u)",
                data[26], data[27], data[28], data[29]);
            }

            if (data[12] == 0x08 && data[13] == 0x00) { // IPv4
                protocol = 1;
            } else if (data[12] == 0x86 && data[13] == 0xDD) { // IPv6
                protocol = 2;
            } else if (data[12] == 0x08 && data[13] == 0x06) { // ARP
                protocol = 3;
            } else {
                protocol = 0;
            }

            snprintf((char*)packet_list[packet_head], MAX_PACKET_STR,
                "%c len=%u SRC=%s DST=%s",
                protocol, header->len, src_mac, dst_mac
            );

            packet_head = (packet_head + 1) % MAX_PACKETS;
            if (packet_count < MAX_PACKETS) packet_count++;

        } else if (res == 0) {
            usleep(100); // нет пакета, ждём
        } else {
            snprintf((char*)packet_list[packet_head], MAX_PACKET_STR, "pcap error or EOF");
            packet_head = (packet_head + 1) % MAX_PACKETS;
            if (packet_count < MAX_PACKETS) packet_count++;
            break;
        }

        while (capture_pause) { usleep(1000); }
    }
    pcap_close(handle);
    return NULL;
}