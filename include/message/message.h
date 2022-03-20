#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

// typedef u64 short_message_t;

static u64 inline __ecall_ebi_listen_message(
	u64 	sender_eid,
	vaddr_t rx_addr,
	usize 	len
)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_LISTEN_MESSAGE,
		sender_eid,
		(u64)rx_addr,
		(u64)len
	);
}

static u64 inline __ecall_ebi_send_message(
	u64		receiver_eid,
	vaddr_t	tx_addr,
	usize	len
)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_SEND_MESSAGE,
		receiver_eid,
		(u64)tx_addr,
		(u64)len
	);
}

static u64 inline __ecall_ebi_stop_listen(u64 sender_eid)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_STOP_LISTEN,
		sender_eid,
		0UL,
		0UL
	);
}