#pragma once

#include <sbi/sbi_trap.h>

int ebi_listen_message_handler(struct sbi_trap_regs* regs);
int ebi_send_message_handler(struct sbi_trap_regs* regs);
int ebi_stop_listen_handler(struct sbi_trap_regs* regs);