/* src: https://lwip.fandom.com/wiki/Porting_For_Bare_Metal */

#pragma once

#define NO_SYS                      1
// #define NO_SYS_NO_TIMERS            1
#define MEM_LIBC_MALLOC             1
#define MEMP_MEM_MALLOC             1
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    (4 * 1024 * 1024)
#define MEMP_NUM_PBUF               1024
#define MEMP_NUM_UDP_PCB            20
#define MEMP_NUM_TCP_PCB            20
#define MEMP_NUM_TCP_PCB_LISTEN     16
#define MEMP_NUM_TCP_SEG            128
#define MEMP_NUM_REASSDATA          32
#define MEMP_NUM_ARP_QUEUE          10
#define PBUF_POOL_SIZE              512
#define LWIP_ARP                    1
#define LWIP_IPV4                   1
#define IP_REASS_MAX_PBUFS          64
#define IP_FRAG_USES_STATIC_BUF     0
#define IP_DEFAULT_TTL              255
#define IP_SOF_BROADCAST            1
#define IP_SOF_BROADCAST_RECV       1
#define LWIP_ICMP                   1
#define LWIP_BROADCAST_PING         1
#define LWIP_MULTICAST_PING         1
#define LWIP_RAW                    1
#define TCP_WND                     (4 * TCP_MSS)
#define TCP_MSS                     1460
#define TCP_SND_BUF                 (8 * TCP_MSS)
#define TCP_LISTEN_BACKLOG          1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1
#define LWIP_NETIF_HWADDRHINT       1
#define LWIP_NETCONN                0
#define LWIP_SOCKET                 0
#define LWIP_STATS_DISPLAY          1
#define MEM_STATS                   0
#define SYS_STATS                   0
#define MEMP_STATS                  0
#define LINK_STATS                  0
#define ETHARP_TRUST_IP_MAC         0
#define ETH_PAD_SIZE                0
#define LWIP_CHKSUM_ALGORITHM       2
#define LWIP_NO_CTYPE_H             1

#define LWIP_TCP_KEEPALIVE          1

// Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing
#define TCP_KEEPIDLE_DEFAULT        10000UL // Default KEEPALIVE timer in milliseconds
#define TCP_KEEPINTVL_DEFAULT       2000UL  // Default Time between KEEPALIVE probes in milliseconds
#define TCP_KEEPCNT_DEFAULT         9U      // Default Counter for KEEPALIVE probes

#define LWIP_DEBUG                  1

#define ETHARP_DEBUG                LWIP_DBG_ON
#define NETIF_DEBUG                 LWIP_DBG_ON
#define PBUF_DEBUG                  LWIP_DBG_ON
#define API_LIB_DEBUG               LWIP_DBG_ON
#define API_MSG_DEBUG               LWIP_DBG_ON
#define SOCKETS_DEBUG               LWIP_DBG_ON
#define ICMP_DEBUG                  LWIP_DBG_ON
#define INET_DEBUG                  LWIP_DBG_ON
#define IP_DEBUG                    LWIP_DBG_ON
#define IP_REASS_DEBUG              LWIP_DBG_ON
#define RAW_DEBUG                   LWIP_DBG_ON
#define MEM_DEBUG                   LWIP_DBG_ON
#define MEMP_DEBUG                  LWIP_DBG_ON
#define SYS_DEBUG                   LWIP_DBG_ON
#define TCP_DEBUG                   LWIP_DBG_ON
#define TCP_INPUT_DEBUG             LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG            LWIP_DBG_ON
#define TCP_RTO_DEBUG               LWIP_DBG_ON
#define TCP_CWND_DEBUG              LWIP_DBG_ON
#define TCP_WND_DEBUG               LWIP_DBG_ON
#define TCP_FR_DEBUG                LWIP_DBG_ON
#define TCP_QLEN_DEBUG              LWIP_DBG_ON
#define TCP_RST_DEBUG               LWIP_DBG_ON
#define UDP_DEBUG                   LWIP_DBG_ON
#define TCPIP_DEBUG                 LWIP_DBG_ON
#define PPP_DEBUG                   LWIP_DBG_ON
#define SLIP_DEBUG                  LWIP_DBG_ON
#define DHCP_DEBUG                  LWIP_DBG_ON

#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_NETIF_EXT_STATUS_CALLBACK  1
#define CONFIG_LWIP_NETIF_STATUS_PRINT  1

#define EMOD_NET_IPADDR_BASE    "10.0.2.15"
#define EMOD_NET_IPADDR_GW      "10.0.2.2"
#define EMOD_NET_IPADDR_NETMASK "255.255.255.0"
