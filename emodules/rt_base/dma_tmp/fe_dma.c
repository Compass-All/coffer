#include "fe_dma.h"
#include "../../be_dma/be_dma.h"
#include "../rt_console.h"
#include "../m3/page_table.h"

#define VIRT_QUEUE_LEN 4
#define VIRT_QUEUE_MOD (VIRT_QUEUE_LEN + 1)

// currently RingBuffer is PDMA specific. should be generalized later
struct RingBuffer {
	uint32_t head;
	uint32_t tail;
	pdma_data buffer[VIRT_QUEUE_LEN];
} virtqueue;

inline int is_empty()
{
	return (virtqueue.head == virtqueue.tail);
}

inline int is_full()
{
	return ((virtqueue.head + 1) % VIRT_QUEUE_MOD == virtqueue.tail);
}

// this function can be used as the call back function
int queue_consume()
{
	if (is_empty()) {
		em_error("ERROR read: virtqueue is empty\n");
		return -1;
	}

	// consuming
	virtqueue.tail = (virtqueue.tail + 1) % VIRT_QUEUE_MOD;

	return 0;
}

int queue_write(pdma_data *data)
{
	if (is_full()) {
		em_error("ERROR write: virtqueue if full\n");
		return -1;
	}

	// writing
	virtqueue.buffer[virtqueue.head] = *data;
	virtqueue.head = (virtqueue.head + 1) % VIRT_QUEUE_MOD;

	return 0;
}

static void dump_virtqueue()
{
	pdma_data *ptr;
	int i;
	uint32_t len = ((virtqueue.head % VIRT_QUEUE_MOD)
		- (virtqueue.tail % VIRT_QUEUE_MOD));

	em_debug("------------\n");
	em_debug("head: %u, tail %u\n", virtqueue.head, virtqueue.tail);
	for (i = 0, ptr = &virtqueue.buffer[virtqueue.tail % VIRT_QUEUE_MOD];
			i < len; i++, ptr++) {
		em_debug("%d: dst: 0x%lx, src: 0x%lx, size: 0x%lx\n",
			i, ptr->dst_addr, ptr->src_addr, ptr->size);
	}
	em_debug("------------\n");
}

void pdma_debug()
{
	pdma_data data1;
	data1.dst_addr = 0x1;
	data1.src_addr = 0x2;
	data1.size = 0x3;

	queue_write(&data1);
	dump_virtqueue();
	queue_write(&data1);
	queue_write(&data1);
	queue_write(&data1);
	dump_virtqueue();
	queue_consume();
	dump_virtqueue();
	queue_consume();
	queue_consume();
	queue_consume();
	dump_virtqueue();

	return;
}

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
 * 
 * 
 * TODO: change all register operations to readx/writex
 * 		 in order to avoid cache influences
 * 		 (currently work around via keyword 'volatile')
 * 
 */

static void pdma_init();
static int claim_avail_channel();
static int try_release_channel(int id);
static int release_channel(int id);
static void pdma_control_page_mapping();
static inline pdma_config pdma_default_config();
static int pdma_try_send_request(int id, pdma_data *data);
static void print_pdma_status();
static void print_pdma_status_id(int id);

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
		volatile pdma_control *ctrl = (pdma_control *)PDMA_CONTROL(i);
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
	volatile pdma_control *ctrl = (pdma_control *)PDMA_CONTROL(id);
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

static inline pdma_config pdma_default_config()
{
	pdma_config config = {0};
	config.rsize = 4;
	config.wsize = 4;

	return config;
}

static int pdma_try_send_request(int id, pdma_data *data)
{
	volatile pdma_control *ctrl = (pdma_control *)PDMA_CONTROL(id);
	volatile pdma_config *config = (pdma_config *)PDMA_NEXT_CONFIG(id);
	
	if (!ctrl->claim) {
		return PDMA_NOT_CLAIMED;
	}

	if (ctrl->run) {
		return PDMA_RUNNING;
	}

	writeq(data->src_addr, (void *)PDMA_NEXT_SRC(id));
	writeq(data->dst_addr, (void *)PDMA_NEXT_DST(id));
	writeq(data->size, (void *)PDMA_NEXT_BYTES(id));

	*config = pdma_default_config();
	ctrl->run = 1;

	return 0;
}

static void print_pdma_status()
{
	for (int i = 0; i < PDMA_NUM; i++) {
		volatile pdma_control ctrl = *(pdma_control *)PDMA_CONTROL(i);
		em_debug("ID: %d\n", i);
		em_debug("claim: %d\n", ctrl.claim);
		em_debug("run: %d\n", ctrl.run);
	}
}

static void print_pdma_status_id(int id)
{
	volatile pdma_control ctrl = *(pdma_control *)PDMA_CONTROL(id);
	uintptr_t src = *(uintptr_t *)PDMA_EXEC_SRC(id);
	uintptr_t dst = *(uintptr_t *)PDMA_EXEC_DST(id);
	uintptr_t size = *(uintptr_t *)PDMA_EXEC_BYTES(id);

	em_debug("INFO channel %d: \n", id);
	em_debug("claim: %d, run: %d, done: %d\n",
		ctrl.claim, ctrl.run, ctrl.done);
	em_debug("src = 0x%lx, dst = 0x%lx, size = 0x%lx\n",
		src, dst, size);
}

static void pdma_test1()
{
	print_pdma_status();
	int id = claim_avail_channel();
	em_debug("channel %d claimed\n", id);
	print_pdma_status();
	release_channel(id);
	em_debug("channel %d released\n", id);
	print_pdma_status();
}

static void pdma_test2()
{
	volatile int *buffer1 = (int *)0x108000000UL;
	volatile int *buffer2 = (int *)0x108001000UL;

	map_page((uintptr_t)buffer1, (uintptr_t)buffer1, 1,
				PTE_V | PTE_W | PTE_R | PTE_D, 0);
	map_page((uintptr_t)buffer2, (uintptr_t)buffer2, 1,
				PTE_V | PTE_W | PTE_R | PTE_D, 0);
	
	int id;
	pdma_data data;
	
	data.src_addr = (uintptr_t)buffer1;
	data.dst_addr = (uintptr_t)buffer2;
	data.size = 40;

	for (int i = 0; i < 256; i++) {
		buffer1[i] = 1;
		buffer2[i] = 2;
	}

	if ( (id = claim_avail_channel()) < 0 ) {
		em_error("Find available channel failed\n");
		return;
	}
	em_debug("channel %d claimed\n", id);
	print_pdma_status_id(id);

	em_debug("try sending request: src 0x%lx, dst 0x%lx, size 0x%lx\n",
		data.src_addr, data.dst_addr, data.size);
	if (pdma_try_send_request(id, &data) < 0) {
		em_error("dma request failed\n");
		return;
	}
	print_pdma_status_id(id);

	release_channel(id);
	em_debug("channel %d released\n", id);
	print_pdma_status_id(id);

	em_debug("src: 0x%lx\n", data.src_addr);
	for (int i = 0; i < 20; i++) {
		em_debug("%d ", buffer1[i]);
		if ((i + 1) % 5 == 0) {
			em_debug("\n");
		}
	}

	em_debug("dst: 0x%lx\n", data.dst_addr);
	for (int i = 0; i < 20; i++) {
		em_debug("%d ", buffer2[i]);
		if ((i + 1) % 5 == 0) {
			em_debug("\n");
		}
	}

}
__attribute__ ((__unused__)) void pdma_debug_unused()
{
	pdma_init();
	pdma_test1();
	pdma_test2();
	return;
}