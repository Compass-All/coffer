#pragma once

#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

err_t virtio_net_init(struct netif *netif);
err_t virtio_net_input(struct pbuf *p, struct netif *netif);
// err_t virtio_net_output(struct netif *netif, struct pbuf *p,
//                         ip_addr_t *ipaddr);
// err_t virtio_net_link_output(struct netif *netif, struct pbuf *p);
void virtio_net_poll(struct netif *netif);