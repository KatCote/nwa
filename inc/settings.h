#ifndef SETTINGS_H_GUARD
#define SETTINGS_H_GUARD

extern volatile char src_mac[12];
extern volatile char dstr_mac[12];

extern volatile char src_ip[12];
extern volatile char dstr_ip[12];

#define C_VLAN "8100"
#define C_IPV4 "0800"
#define C_ARP  "0806"
#define C_IPV6 "86DD"

#endif