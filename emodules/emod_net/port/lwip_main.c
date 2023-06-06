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
#include "virtio_net.h"

#define EMOD_NET_IPADDR_BASE    "192.168.0.2"
#define EMOD_NET_IPADDR_GW      "192.168.0.1"
#define EMOD_NET_IPADDR_NETMASK "255.255.255.0"

ip4_addr_t emod_net_ip_addr;

#define EMOD_NET_ARP_TIMER_INTERVAL      (ARP_TMR_INTERVAL * 1000)
#define EMOD_NET_TCP_TIMER_INTERVAL      (TCP_TMR_INTERVAL * 1000)
#define EMOD_NET_IPREASS_TIMER_INTERVAL  (IP_TMR_INTERVAL * 1000)

static emod_net_timestamp ts_etharp;
static emod_net_timestamp ts_tcp;
static emod_net_timestamp ts_ipreass;

static struct netif virtio_netif;

// Functions from my netif driver
// Probe function (find the device, return driver private data)
extern int virtio_net_probe(struct pci_dev *, void **, u8_t *);
// Init function
// extern int virtio_net_attach(struct netif *);
// Poll for received frames
extern void virtio_net_poll(struct netif *);

int emod_net_aton(char * str_addr, ip4_addr_t * net_addr);

int emod_net_init(void)
{
    ip4_addr_t gw_addr, netmask;
    struct pci_dev * pcidev;
    void * emod_net_state;
    u8_t mac_addr[6];
    int err = -1;

    // Hard-coded IP for my address, gateway and netmask
    if (emod_net_aton(EMOD_NET_IPADDR_BASE, &emod_net_ip_addr))
        return -1;
    if (emod_net_aton(EMOD_NET_IPADDR_GW, &gw_addr))
        return -1;
    if (emod_net_aton(EMOD_NET_IPADDR_NETMASK, &netmask))
        return -1;

    // Initialize LWIP
    lwip_init();

    // // Initialize PCI bus structure
    // pci_init();

    // // Search through the list of PCI devices until we find our NIC
    // pcidev = NULL;
    // while ((pcidev = pci_next(pcidev)) != NULL) {
    //     if ((err = virtio_net_probe(pcidev, &emod_net_state, mac_addr)) == 0)
    //         break;
    // }

    // if (pcidev == NULL) {
    //     printf("emod_net_init: network adapter not found\n");
    //     return -1;
    // }

// struct *netif netif_add(struct netif *mynetif, struct ip_addr *ipaddr,
//                         struct ip_addr *netmask, struct ip_addr *gw,
//                         void *state,
//                         err_t (* init)(struct netif *netif), 
//                         err_t (* input)(struct pbuf *p, struct netif *netif));
    // Add our netif to LWIP (netif_add calls our driver initialization function)
    if (netif_add(&virtio_netif, &emod_net_ip_addr, &netmask, &gw_addr,
                emod_net_state, virtio_net_init, ethernet_input) == NULL) {
        printf("emod_net_init: netif_add (virtio_net_init) failed\n");
        return -1;
    }

    netif_set_default(&virtio_netif);
    netif_set_up(&virtio_netif);

    // Initialize timer values
    emod_net_timestamp_get(&ts_etharp);
    emod_net_timestamp_get(&ts_tcp);
    emod_net_timestamp_get(&ts_ipreass);

    return 0;
}

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
    virtio_net_poll(&virtio_netif);

    sys_check_timeouts();
    // // Process lwip network-related timers.
    // emod_net_timestamp_get(&now);
    // if (emod_net_timestamp_diff(&ts_etharp, &now) >= EMOD_NET_ARP_TIMER_INTERVAL) {
    //     etharp_tmr();
    //     ts_etharp = now;
    // }
    // if (emod_net_timestamp_diff(&ts_tcp, &now) >= EMOD_NET_TCP_TIMER_INTERVAL) {
    //     tcp_tmr();
    //     ts_tcp = now;
    // }
    // if (emod_net_timestamp_diff(&ts_ipreass, &now) >= EMOD_NET_IPREASS_TIMER_INTERVAL) {
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
    struct in_addr a;
    int i = inet_aton(str_addr, &net_addr->addr);
    if (!i)
        return -1;
    return 0;
}

void emod_net_wait_for_interrupt()
{
    // Wait for interrupt
    sleep(1);
}

//
// Main entry point
//
int lwip_main(void)
{
    // [snip other non-lwip initializations]
    emod_net_timestamp_init();       // Initialize timestamp generator
    emod_net_init();
    while (1) {
        // [snip other non-lwip functions]
        emod_net_wait_for_interrupt();   // Awakened by network, timer or other interrupt
        emod_net_poll();             // Poll network stack
    }
}