emodule-objs-y = 
emodule-objs-y += emod_net.o 
emodule-objs-y += dependency.o 

emodule-objs-y += lwip/src/api/err.o

emodule-objs-y += lwip/src/core/mem.o
emodule-objs-y += lwip/src/core/memp.o
emodule-objs-y += lwip/src/core/timeouts.o
emodule-objs-y += lwip/src/core/netif.o
emodule-objs-y += lwip/src/core/pbuf.o
emodule-objs-y += lwip/src/core/raw.o
emodule-objs-y += lwip/src/core/stats.o
emodule-objs-y += lwip/src/core/sys.o
emodule-objs-y += lwip/src/core/tcp.o
emodule-objs-y += lwip/src/core/tcp_in.o
emodule-objs-y += lwip/src/core/tcp_out.o
emodule-objs-y += lwip/src/core/udp.o
emodule-objs-y += lwip/src/core/init.o
emodule-objs-y += lwip/src/core/ip.o
emodule-objs-y += lwip/src/core/inet_chksum.o

emodule-objs-y += lwip/src/core/ipv4/icmp.o
emodule-objs-y += lwip/src/core/ipv4/ip4.o
emodule-objs-y += lwip/src/core/ipv4/dhcp.o
emodule-objs-y += lwip/src/core/ipv4/ip4_addr.o
emodule-objs-y += lwip/src/core/ipv4/ip4_frag.o
emodule-objs-y += lwip/src/core/ipv4/etharp.o
emodule-objs-y += lwip/src/core/ipv4/acd.o

emodule-objs-y += lwip/src/netif/ethernet.o

emodule-objs-y += port/common/netbuf.o
emodule-objs-y += port/common/sglist.o
emodule-objs-y += port/common/string.o

emodule-objs-y += port/virtio_net/bus.o
emodule-objs-y += port/virtio_net/fdt.o
emodule-objs-y += port/virtio_net/fdt_addresses.o
emodule-objs-y += port/virtio_net/fdt_ro.o
emodule-objs-y += port/virtio_net/ofw_fdt.o
emodule-objs-y += port/virtio_net/pci_bus.o
emodule-objs-y += port/virtio_net/pci_bus_riscv64.o
emodule-objs-y += port/virtio_net/pci_ecam.o
emodule-objs-y += port/virtio_net/platform_bus.o
emodule-objs-y += port/virtio_net/virtio_bus.o
emodule-objs-y += port/virtio_net/virtio_net.o
emodule-objs-y += port/virtio_net/virtio_pci.o
emodule-objs-y += port/virtio_net/virtio_ring.o
emodule-objs-y += port/lwip_init.o
emodule-objs-y += port/lwip_main.o
emodule-objs-y += port/lwip_netbuf.o
emodule-objs-y += port/netdev.o
emodule-objs-y += port/sys_arch.o
emodule-objs-y += port/timestamp.o
emodule-objs-y += port/uk_netdev.o