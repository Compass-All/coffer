#include "memory/memory.h"
#include "util/register.h"
#include <types.h>
#include <sbi/ebi/message_channel.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/sbi_system.h>
#include <sbi/ebi/memory_util.h>
#include <sbi/ebi/eval.h>
#include <sbi/riscv_asm.h>
#include <memory/page_table.h>
#include <enclave/eid.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_unpriv.h>

typedef struct {
	vaddr_t tx_vaddr;
	vaddr_t rx_vaddr;

	usize len;

#define MESSAGE_CHANNEL_IDLE	0
#define MESSAGE_CHANNEL_INUSE	1
	u8 status;
} message_channel_t;

#define CHANNEL_TYPE_TX	0
#define CHANNEL_TYPE_RX	1

static message_channel_t rx_channel[NUM_ENCLAVE + 1];
static message_channel_t tx_channel[NUM_ENCLAVE + 1];

static message_channel_t *get_message_channel(u64 eid, u8	type)
{
	if (eid > NUM_ENCLAVE)
		panic("Invalid eid\n");

	message_channel_t *channel = NULL;

	if (type == CHANNEL_TYPE_RX) {
		channel = &rx_channel[eid];
	} else if (type == CHANNEL_TYPE_TX) {
		channel = &tx_channel[eid];
	} else
		panic("Invalid channel type\n");
	
	return channel;
}

static void start_message_channel(
	u64		eid,
	u8		type,
	vaddr_t rx_vaddr,
	usize	len
)
{
	message_channel_t *channel = get_message_channel(eid, type);

	show(eid); show(type); show(rx_vaddr); show(len);

	if (channel->status != MESSAGE_CHANNEL_IDLE) {
		sbi_error("channel already in use\n");
        panic("Stall\n");
		return;
	}

	channel->rx_vaddr	= rx_vaddr;
	channel->len		= len;
	channel->status		= MESSAGE_CHANNEL_INUSE;
}

static int copy_message(message_channel_t *channel, u64 rx_eid)
{
	show(rx_eid);
    show(get_current_mpp());

	u64 satp = get_enclave_satp(rx_eid);
	pte_t *page_table_root = (pte_t *) ((satp << 20) >> 8);
	if (satp == 0)
		panic("Message can only be copied when MMU is on\n");
	// rx_paddr = get_pa(page_table_root, rx_vaddr);

	vaddr_t rx_vaddr = channel->rx_vaddr, tx_vaddr = channel->tx_vaddr;
	paddr_t rx_paddr;
	usize len = channel->len;

	usize gap = PAGE_UP(rx_vaddr) - rx_vaddr;
	if (len < gap)
		gap = len;
	if (gap) {
		rx_paddr = get_pa(page_table_root, rx_vaddr);
		sbi_debug("gap: 0x%lx, "
			"rx_paddr = 0x%lx, rx_vaddr = 0x%lx, tx_vaddr = 0x%lx\n",
			gap, rx_paddr, rx_vaddr, tx_vaddr);
		copy_from_user_not_aligned(rx_paddr, tx_vaddr, gap);
	}
	len -= gap;
	rx_vaddr += gap;
	tx_vaddr += gap;

	if (!len)
		return 0;
	if (rx_vaddr % PAGE_SIZE)
		panic("not aligned\n");

	while (len >= PAGE_SIZE) {
		rx_paddr = get_pa(page_table_root, rx_vaddr);
		sbi_debug("copy page: rx_paddr = 0x%lx, rx_vaddr = 0x%lx, tx_vaddr = 0x%lx\n",
			rx_paddr, rx_vaddr, tx_vaddr);
		copy_from_user_not_aligned(rx_paddr, tx_vaddr, PAGE_SIZE);
		len -= PAGE_SIZE;
		rx_vaddr += PAGE_SIZE;
		tx_vaddr += PAGE_SIZE;
	}

	if (len) {
		rx_paddr = get_pa(page_table_root, rx_vaddr);
		sbi_debug("copy rest: len: 0x%lx, rx_paddr = 0x%lx, rx_vaddr = 0x%lx, tx_vaddr = 0x%lx\n",
			len, rx_paddr, rx_vaddr, tx_vaddr);
		copy_from_user_not_aligned(rx_paddr, tx_vaddr, len);
	}

	return 0;
}
static void reset_channel(message_channel_t *channel)
{
	channel->tx_vaddr	= 0;
	channel->rx_vaddr 	= 0;
	channel->len 		= 0;
	channel->status		= MESSAGE_CHANNEL_IDLE;
}

static void finish_message_channel(
	u64		eid,
	u8		type,
	vaddr_t	tx_vaddr,
	usize	len
)
{
	message_channel_t *channel = get_message_channel(eid, type);

	show(eid); show(type); show(tx_vaddr); show(len);

	if (channel->status != MESSAGE_CHANNEL_INUSE) {
		sbi_error("channel not started\n");
		return;
	}

	if (len > channel->len) {
		sbi_error("message longer than expected!\n");
		return;
	}

	channel->tx_vaddr	= tx_vaddr;
	channel->len		= len;

	u64 rx_eid = (type == CHANNEL_TYPE_TX) ? HOST_EID : eid;
	int ret = copy_message(channel, rx_eid);
	if (ret)
		panic("Message copy error\n");

	reset_channel(channel);
}

static int check_privilege_mode()
{
	u64 mstatus = csr_read(CSR_MSTATUS);
	u64 mpp 	= (mstatus & (0b11UL << MSTATUS_MPP_SHIFT))
					>> MSTATUS_MPP_SHIFT;

	if (mpp != 1UL) {
		// there is a bug here

		// LOG(get_current_eid());
		// LOG(mpp);
		// panic("Message channel cannot be started in U mode enclave\n");
	}

	return 0;
}

int ebi_listen_message_handler(struct sbi_trap_regs* regs)
{
	u64		eid			= regs->a0;
	vaddr_t rx_vaddr	= regs->a1;
	usize	len			= regs->a2;

	u64 current_eid = get_current_eid();
	show(current_eid);

	START_TIMER(message, current_eid);

	if (current_eid != HOST_EID)
		check_privilege_mode();

	u8 type;
	if (current_eid == HOST_EID) {
		type = CHANNEL_TYPE_TX;
	} else {
		type = CHANNEL_TYPE_RX;
		eid = current_eid;
	}

	start_message_channel(
		eid,
		type,
		rx_vaddr,
		len
	);

	STOP_TIMER(message, current_eid);

	return 0;
}

int ebi_send_message_handler(struct sbi_trap_regs* regs)
{
	u64 	eid			= regs->a0;
	vaddr_t tx_vaddr	= regs->a1;
	usize	len			= regs->a2;

	u64 current_eid = get_current_eid();
    show(current_eid);

	START_TIMER(message, current_eid);

	u8 type;
	if (current_eid == HOST_EID) {
		type = CHANNEL_TYPE_RX;
	} else {
		type = CHANNEL_TYPE_TX;
		eid = current_eid;
	}

	finish_message_channel(
		eid,
		type,
		tx_vaddr,
		len
	);

	STOP_TIMER(message, current_eid);

	return 0;
}

int ebi_stop_listen_handler(struct sbi_trap_regs* regs)
{
	u64 eid = regs->a0;

	u64 current_eid = get_current_eid();

	START_TIMER(message, current_eid);

	message_channel_t *channel;
	if (current_eid == HOST_EID)
		channel = get_message_channel(eid, CHANNEL_TYPE_TX);
	else 
		channel = get_message_channel(current_eid, CHANNEL_TYPE_RX);

	reset_channel(channel);

	STOP_TIMER(message, eid);

	return 0;
}