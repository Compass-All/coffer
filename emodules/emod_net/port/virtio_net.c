#include "../dependency.h"
#include "virtio_net.h"

static err_t virtio_net_output(struct netif *netif, struct pbuf *p,
    const struct ip4_addr *ipaddr)
{
    debug("Dummy output\n");
    return ERR_OK;
}

static err_t virtio_net_link_output(struct netif *netif, struct pbuf *p)
{
    debug("Dummy link output\n");
    return ERR_OK;
}

err_t virtio_net_init(struct netif *netif)
{
    debug("netif @ %p\n", netif);

    netif->output = virtio_net_output;
    netif->linkoutput = virtio_net_link_output;

    return ERR_OK;
}

void virtio_net_poll(struct netif *netif)
{
    debug("Dummy poll\n");
}