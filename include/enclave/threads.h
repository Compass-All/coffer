#pragma  once

#define MAX_THREADS  32 // must be less than or equal to XLEN (bitmap limit)

#define FUTEX_WAIT		0
#define FUTEX_WAKE		1
