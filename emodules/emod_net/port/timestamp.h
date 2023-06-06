#pragma once

#include <types.h>

typedef u64 emod_net_timestamp;

void emod_net_timestamp_init();
void emod_net_timestamp_get(emod_net_timestamp *ts);
s64 emod_net_timestamp_diff(emod_net_timestamp *ts1,
        emod_net_timestamp *ts2);
void sleep(u32 ms);