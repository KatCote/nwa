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
    pcap_t *handle = pcap_open_live("enp7s0", MAX_PACKET_STR, 1, 100, errbuf);
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
            char pdata[MAX_PACKET_STR] = {0};
            int offset = 0;

            for (int i = 0; i < header->caplen; i++)
            {
                int ret = snprintf(pdata + offset, (header->caplen * 2) - offset, "%02X", data[i]);
                if (ret < 0) { break; } else { offset += ret; }
            }

            snprintf((char*)packet_list[packet_head], MAX_PACKET_STR,
                "len=%u\tcaplen=%u\tts=%ld.%06ld\tdata=%.*s",
                header->len, header->caplen, header->ts.tv_sec, header->ts.tv_usec, (header->caplen * 2), pdata
            );

            packet_head = (packet_head + 1) % MAX_PACKETS;
            if (packet_count < MAX_PACKETS) packet_count++;

        } else if (res == 0) {
            usleep(100);
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