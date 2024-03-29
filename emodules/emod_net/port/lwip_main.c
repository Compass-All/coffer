#include "lwip_main.h"

#include "../dependency.h"
#include "timestamp.h"
#include "arch/cc.h"
#include "lwip/timeouts.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/ip_addr.h"
#include "lwip/ip4_frag.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/priv/tcp_priv.h"
#include "netif/etharp.h"
#include "virtio_net/virtio_net.h"
#include <util/gnu_attribute.h>

ip4_addr_t emod_net_ip_addr;

#define EMOD_NET_ARP_TIMER_INTERVAL      (ARP_TMR_INTERVAL * 1000)
#define EMOD_NET_TCP_TIMER_INTERVAL      (TCP_TMR_INTERVAL * 1000)
#define EMOD_NET_IPREASS_TIMER_INTERVAL  (IP_TMR_INTERVAL * 1000)

// static emod_net_timestamp ts_etharp;
// static emod_net_timestamp ts_tcp;
// static emod_net_timestamp ts_ipreass;

// static struct netif virtio_netif;

// Poll for received frames
// extern void virtio_net_poll(struct netif *);

int emod_net_aton(char * str_addr, ip4_addr_t * net_addr);

//
// Regular polling mechanism.  This should be called each time through
// the main application loop (after each interrupt, regardless of source).
//
// It handles any received packets, permits NIC device driver house-keeping
// and invokes timer-based TCP/IP functions (TCP retransmissions, delayed
// acks, IP reassembly timeouts, ARP timeouts, etc.)
//
void emod_net_poll(void)
{
    emod_net_timestamp now;

    // Call network interface to process incoming packets and do housekeeping
    // virtio_net_poll(&virtio_netif);

    emod_net_timestamp_get(&now);

    // debug("now: %lu\n", now);

    sys_check_timeouts();
    // Process lwip network-related timers.
    // if (emod_net_timestamp_diff(&ts_etharp, &now) >= EMOD_NET_ARP_TIMER_INTERVAL) {
    //     debug("etharp timeout\n");
    //     etharp_tmr();
    //     ts_etharp = now;
    // }
    // if (emod_net_timestamp_diff(&ts_tcp, &now) >= EMOD_NET_TCP_TIMER_INTERVAL) {
    //     debug("tcp timeout\n");
    //     tcp_tmr();
    //     ts_tcp = now;
    // }
    // if (emod_net_timestamp_diff(&ts_ipreass, &now) >= EMOD_NET_IPREASS_TIMER_INTERVAL) {
    //     debug("ip_reass timeout\n");
    //     ip_reass_tmr();
    //     ts_ipreass = now;
    // }
}

//
// Convert address from string to internal format.
// Return 0 on success; else non-zero
//
int emod_net_aton(char * str_addr, ip4_addr_t * net_addr)
{
    int i = inet_aton(str_addr, &net_addr->addr);
    if (!i)
        return -1;
    return 0;
}

void emod_net_wait_for_interrupt()
{
    // Wait for interrupt
    sleep(500);
}

//
// Main entry point
//
int lwip_main(void)
{
    // [snip other non-lwip initializations]
    while (1) {
        // [snip other non-lwip functions]
        emod_net_wait_for_interrupt();   // Awakened by network, timer or other interrupt
        emod_net_poll();             // Poll network stack
    }
}