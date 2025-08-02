#ifndef TRACER_H_GUARD
#define TRACER_H_GUARD

/* D E F I N E S */

#define MAX_PACKETS 256
#define MAX_PACKET_STR 1518

extern volatile char packet_list[MAX_PACKETS][MAX_PACKET_STR];
extern volatile int packet_count;
extern volatile int packet_head;

#endif