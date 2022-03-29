#pragma once

#define SBI_EXT_EBI 0x19260817

/* SBI function IDs for EBI extension*/
#define SBI_EXT_EBI_START 398
#define SBI_EXT_EBI_CREATE 399
#define SBI_EXT_EBI_ENTER 400
#define SBI_EXT_EBI_EXIT 401
#define SBI_EXT_EBI_SUSPEND 403
#define SBI_EXT_EBI_RESUME 404
#define SBI_EXT_EBI_MEM_ALLOC 405
#define SBI_EXT_EBI_MAP_REGISTER 406

#define SBI_EXT_EBI_PUTS 410
#define SBI_EXT_EBI_GETS 411

#define SBI_EXT_EBI_PERI_INFORM 420
#define SBI_EXT_EBI_FETCH 421
#define SBI_EXT_EBI_RELEASE 422

#define SBI_EXT_EBI_FLUSH_DCACHE 430
#define SBI_EXT_EBI_DISCARD_DCACHE 431

#define SBI_EXT_EBI_LISTEN_MESSAGE 440
#define SBI_EXT_EBI_SEND_MESSAGE 441
#define SBI_EXT_EBI_STOP_LISTEN 442

#define SBI_EXT_EBI_GET_TIME 460

#define SBI_EXT_EBI_DEBUG 499
