#include "dma.h"
#include "../rt_console.h"
#include "../m3/page_table.h"


/**
 * PDMA State Machine
 * 
 * 		    set CLAIM				  set RUN
 * IDLE  ------------>  CLAIMED  ------------>  RUNNING
 * 		 <------------			 <------------	
 * 		  clear CLAIM			  job finished
 *								   (passively)
 *
 * Note: race conditions are out-of-scope.
 * 		 functions in this module should not be used under
 * 		 multi-threading.
 */


static void pdma_init();
static int claim_avail_channel();
static int try_release_channel(int id);
static int release_channel(int id);
static void pdma_control_page_mapping();
static void print_pdma_status();


static void pdma_init()
{
	pdma_control_page_mapping();
}

/**
 * @brief Find an available channel and retrun its ID.
 * 
 * @return int: ID of the available channel. PDMA_NO_AVAIL if not found.
 */
static int claim_avail_channel()
{
	for (int i = 0; i < PDMA_NUM; i++) {
		pdma_control *ctrl = (pdma_control *)PDMA_CONTROL(i);
		if (!ctrl->claim) {
			ctrl->claim = 1;
			return i;
		}
	}
	return PDMA_NO_AVAIL;
}

/**
 * @brief Try to release a claimed channel
 * 
 * @param id: id of the channel
 * @return int: 0 on success; PDMA_ERROR if the channel is not claimed;
 * 		   	    PDMA_RUNNING if the channel has an unfinished job
 */
static int try_release_channel(int id)
{
	pdma_control *ctrl = (pdma_control *)PDMA_CONTROL(id);
	if (!ctrl->claim) {
		em_error("Channel %d is not claimed\n", id);
		return PDMA_ERROR;
	}

	if (!ctrl->run) {
		ctrl->claim = 0;
		return 0;
	}

	return PDMA_RUNNING;
}

static int release_channel(int id)
{
	int ret;
	while ((ret = try_release_channel(id)) < 0) {
		if (ret == PDMA_ERROR)
			return PDMA_ERROR;
	}

	return 0;
}

static void pdma_control_page_mapping()
{
	uintptr_t addr = PDMA_BASE_ADDR;
	uintptr_t n_pages = PDMA_SIZE >> EPAGE_SHIFT;
	map_page(addr, addr, n_pages, PTE_V | PTE_W | PTE_R | PTE_D, 0);
}

static void print_pdma_status()
{
	for (int i = 0; i < PDMA_NUM; i++) {
		pdma_control ctrl = *(pdma_control *)PDMA_CONTROL(i);
		em_debug("ID: %d\n", i);
		em_debug("claim: %d\n", ctrl.claim);
		em_debug("run: %d\n", ctrl.run);
	}
}

void pdma_debug()
{
	pdma_init();
	print_pdma_status();
	int id = claim_avail_channel();
	em_debug("channel %d claimed\n", id);
	print_pdma_status();
	release_channel(id);
	em_debug("channel %d released\n", id);
	print_pdma_status();
	return;
}