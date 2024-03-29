#pragma once

#define SBI_EXT_EBI 0x19260817

/* SBI function IDs for EBI extension*/
#define SBI_EXT_EBI_START 						398
#define SBI_EXT_EBI_CREATE 						399
#define SBI_EXT_EBI_ENTER 						400
#define SBI_EXT_EBI_EXIT 						401
#define SBI_EXT_EBI_SUSPEND 					403
#define SBI_EXT_EBI_RESUME 						404
#define SBI_EXT_EBI_MEM_ALLOC 					405
#define SBI_EXT_EBI_ADDR_RECORD 				406

#define SBI_EXT_EBI_PERI_INFORM 				420
#define SBI_EXT_EBI_FETCH 						421
#define SBI_EXT_EBI_RELEASE 					422

#define SBI_EXT_EBI_FLUSH_DCACHE 				430
#define SBI_EXT_EBI_DISCARD_DCACHE 				431

#define SBI_EXT_EBI_LISTEN_MESSAGE 				440
#define SBI_EXT_EBI_SEND_MESSAGE 				441
#define SBI_EXT_EBI_STOP_LISTEN 				442

#define SBI_EXT_EBI_GET_EID		 				450
#define SBI_EXT_EBI_GET_HARTID	 				451

#define SBI_EXT_EBI_DEBUG 						499

#define SBI_EXT_EBI_DEBUG_DUMP_STATUS			500
#define SBI_EXT_EBI_DEBUG_DUMP_OWNERSHIP		501
#define SBI_EXT_EBI_DEBUG_DUMP_PMP				502
#define SBI_EXT_EBI_DEBUG_DUMP_REGION			503
#define SBI_EXT_EBI_GET_ALIVE_COUNT				504
#define SBI_EXT_EBI_GET_STATUS					505
#define SBI_EXT_EBI_DEBUG_UNMATCHED_ACC_FAULT	506

#define SBI_EXT_EVAL_SET_S_TIMER				600
#define SBI_EXT_EVAL_GET_TIMER					601

#define SBI_EXT_EBI_RESET						1000
#define SBI_EXT_GET_EID_COUND					1001