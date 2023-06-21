/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2008 Yahoo!, Inc.
 * All rights reserved.
 * Written by: John Baldwin <jhb@FreeBSD.org>
 *	       Sharan Santhanam <sharan.santhanam@neclab.eu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/**
 * Taken and modified from FreeBSD
 * Commit id: 4736ccfd9c34
 */

#include <types.h>
#include "sglist.h"
#include <util/errno.h>
#include <memory/memory.h>
#include <util/arithmetic.h>
#include <arch/sys_arch.h>
// #include <string.h>
// #include <uk/plat/io.h>
// #include <uk/arch/limits.h>
// #include <uk/refcount.h>
// #include <uk/assert.h>
// #ifdef CONFIG_LIBUKALLOC
// #include <uk/alloc.h>
// #endif /* CONFIG_LIBUKALLOC */

/*
 * Convenience macros to save the state of an sglist so it can be restored
 * if an append attempt fails.  Since sglist's only grow we only need to
 * save the current count of segments and the length of the ending segment.
 * Earlier segments will not be changed by an append, and the only change
 * that can occur to the ending segment is that it can be extended.
 */
struct sgsave {
	__u16 sg_nseg;
	size_t ss_len;
};

#define page_off(x)    ((unsigned long)(x) & (__PAGE_SIZE - 1))
#define trunc_page(x)    ((unsigned long)(x) & (__PAGE_MASK))

#define	SGLIST_SAVE(sg, sgsave) do {					\
	(sgsave).sg_nseg = (sg)->sg_nseg;				\
	if ((sgsave).sg_nseg > 0)					\
		(sgsave).ss_len = (sg)->sg_segs[(sgsave).sg_nseg - 1].ss_len; \
	else								\
		(sgsave).ss_len = 0;					\
} while (0)

#define	SGLIST_RESTORE(sg, sgsave) do {					\
	(sg)->sg_nseg = (sgsave).sg_nseg;				\
	if ((sgsave).sg_nseg > 0)					\
		(sg)->sg_segs[(sgsave).sg_nseg - 1].ss_len = (sgsave).ss_len; \
} while (0)

static inline int _sglist_append_range(struct uk_sglist *sg,
			struct uk_sglist_seg **ssp, __paddr_t paddr,
			size_t len);
static inline int _sglist_append_buf(struct uk_sglist *sg, void *buf,
				size_t len, size_t *donep);
/**
 * Append a single (paddr, len) to a sglist.  sg is the list and ss is
 * the current segment in the list.  If we run out of segments then
 * EFBIG will be returned.
 */
static inline int _sglist_append_range(struct uk_sglist *sg,
			struct uk_sglist_seg **ssp, __paddr_t paddr,
			size_t len)
{
	struct uk_sglist_seg *ss;

	ss = *ssp;
	if (ss->ss_paddr + ss->ss_len == paddr)
		ss->ss_len += len;
	else {
		if (sg->sg_nseg == sg->sg_maxseg)
			return -EFBIG;
		ss++;
		ss->ss_paddr = paddr;
		ss->ss_len = len;
		sg->sg_nseg++;
		*ssp = ss;
	}
	return 0;
}

/**
 * Worker routine to append a virtual address range (either kernel or
 * user) to a scatter/gather list.
 */
static inline int _sglist_append_buf(struct uk_sglist *sg, void *buf,
				size_t len, size_t *donep)
{
	struct uk_sglist_seg *ss;
	__vaddr_t vaddr, offset;
	__paddr_t paddr;
	size_t seglen;
	int error;

	if (donep)
		*donep = 0;
	if (len == 0)
		return 0;

	/* Do the first page.  It may have an offset. */
	vaddr = (__vaddr_t)buf;
	offset = page_off(vaddr);
	paddr = ukplat_virt_to_phys((void *)vaddr);
	seglen = MIN(len, __PAGE_SIZE - offset);
	if (sg->sg_nseg == 0) {
		ss = sg->sg_segs;
		ss->ss_paddr = paddr;
		ss->ss_len = seglen;
		sg->sg_nseg = 1;
	} else {
		ss = &sg->sg_segs[sg->sg_nseg - 1];
		error = _sglist_append_range(sg, &ss, paddr, seglen);
		if (error)
			return error;
	}
	vaddr += seglen;
	len -= seglen;
	if (donep)
		*donep += seglen;

	while (len > 0) {
		seglen = MIN(len, __PAGE_SIZE);
		paddr = ukplat_virt_to_phys((void *)vaddr);
		error = _sglist_append_range(sg, &ss, paddr, seglen);
		if (error)
			return error;
		vaddr += seglen;
		len -= seglen;
		if (donep)
			*donep += seglen;
	}

	return 0;
}

int uk_sglist_count(void *buf, size_t len)
{
	__vaddr_t vaddr, vendaddr;
	__paddr_t lastaddr, paddr;
	int nsegs;

	if (len == 0)
		return 0;

	vaddr = trunc_page((__vaddr_t)buf);
	vendaddr = (__vaddr_t)buf + len;
	nsegs = 1;
	lastaddr = ukplat_virt_to_phys((void *)vaddr);
	vaddr += __PAGE_SIZE;
	while (vaddr < vendaddr) {
		paddr = ukplat_virt_to_phys((void *)vaddr);
		if (lastaddr + __PAGE_SIZE != paddr)
			nsegs++;
		lastaddr = paddr;
		vaddr += __PAGE_SIZE;
	}
	return nsegs;
}

int uk_sglist_append(struct uk_sglist *sg, void *buf, size_t len)
{
	struct sgsave save;
	int error;

	ASSERT(sg);

	if (sg->sg_maxseg == 0)
		return -EINVAL;

	SGLIST_SAVE(sg, save);
	error = _sglist_append_buf(sg, buf, len,  NULL);
	if (error)
		SGLIST_RESTORE(sg, save);

	return error;
}

int uk_sglist_append_sglist(struct uk_sglist *sg,
			const struct uk_sglist *source,
			size_t offset, size_t length)
{
	struct sgsave save;
	struct uk_sglist_seg *ss;
	size_t seglen;
	int error, i;

	ASSERT(sg);

	if (sg->sg_maxseg == 0 || length == 0)
		return -EINVAL;
	SGLIST_SAVE(sg, save);
	error = -EINVAL;
	ss = &sg->sg_segs[sg->sg_nseg - 1];
	for (i = 0; i < source->sg_nseg; i++) {
		if (offset >= source->sg_segs[i].ss_len) {
			offset -= source->sg_segs[i].ss_len;
			continue;
		}
		seglen = source->sg_segs[i].ss_len - offset;
		if (seglen > length)
			seglen = length;
		error = _sglist_append_range(sg, &ss,
		    source->sg_segs[i].ss_paddr + offset, seglen);
		if (error)
			break;
		offset = 0;
		length -= seglen;
		if (length == 0)
			break;
	}
	if (length != 0) {
		error("Failed to add buffer to the sg list\n");
		error = -EINVAL;
	}
	if (error)
		SGLIST_RESTORE(sg, save);
	return error;
}

size_t uk_sglist_length(struct uk_sglist *sg)
{
	size_t space;
	int i;

	ASSERT(sg);

	space = 0;
	for (i = 0; i < sg->sg_nseg; i++)
		space += sg->sg_segs[i].ss_len;
	return space;
}

int uk_sglist_join(struct uk_sglist *first, struct uk_sglist *second)
{
	struct uk_sglist_seg *flast, *sfirst;
	int append;

	/* If 'second' is empty, there is nothing to do. */
	if (second->sg_nseg == 0)
		return 0;

	/*
	 * If the first entry in 'second' can be appended to the last entry
	 * in 'first' then set append to '1'.
	 */
	append = 0;
	flast = &first->sg_segs[first->sg_nseg - 1];
	sfirst = &second->sg_segs[0];
	if (first->sg_nseg != 0 &&
	    flast->ss_paddr + flast->ss_len == sfirst->ss_paddr)
		append = 1;

	/* Make sure 'first' has enough room. */
	if (first->sg_nseg + second->sg_nseg - append > first->sg_maxseg)
		return -EFBIG;

	/* Merge last in 'first' and first in 'second' if needed. */
	if (append)
		flast->ss_len += sfirst->ss_len;

	/* Append new segments from 'second' to 'first'. */
	memmove(second->sg_segs + append, first->sg_segs + first->sg_nseg,
	    (second->sg_nseg - append) * sizeof(struct uk_sglist_seg));
	first->sg_nseg += second->sg_nseg - append;
	uk_sglist_reset(second);
	return 0;
}

struct uk_sglist *uk_sglist_alloc(int nsegs)
{
	struct uk_sglist *sg;

	ASSERT(a);

	/* Allocate the scatter/gather list */
	sg = malloc(sizeof(struct uk_sglist) +
			(nsegs * sizeof(struct uk_sglist_seg)));
	if (!sg) {
		error("Error in allocating sg list\n");
		return NULL;
	}

	/* Initialize the list */
	uk_sglist_init(sg, nsegs, (struct uk_sglist_seg *)(sg + 1));
	return sg;
}

void uk_sglist_free(struct uk_sglist *sg)
{
	ASSERT(sg && a);

	if (uk_refcount_release(&sg->sg_refs))
		free(sg);
}

struct uk_sglist *uk_sglist_build(void *buf,
				size_t len)
{
	struct uk_sglist *sg;
	int nsegs;

	ASSERT(a);

	if (len == 0)
		return NULL;

	nsegs = uk_sglist_count(buf, len);
	ASSERT(nsegs > 0);

	sg = uk_sglist_alloc(nsegs);
	if (sg == NULL) {
		error("Error in allocating the sg list\n");
		return NULL;
	}
	if (uk_sglist_append(sg, buf, len) != 0) {
		error("Error in adding segments to sg list\n");
		uk_sglist_free(sg);
		return NULL;
	}
	return sg;
}

struct uk_sglist *uk_sglist_clone(struct uk_sglist *sg)
{
	struct uk_sglist *new;

	ASSERT(a);

	if (!sg)
		return NULL;

	new = uk_sglist_alloc(sg->sg_maxseg);
	if (!new) {
		error("Allocation for the new list failed\n");
		return NULL;
	}

	new->sg_nseg = sg->sg_nseg;
	memmove(new->sg_segs, sg->sg_segs,
			sizeof(struct uk_sglist_seg) * sg->sg_nseg);
	return new;
}

int uk_sglist_split(struct uk_sglist *original, struct uk_sglist **head,
			size_t length)
{
	struct uk_sglist *sg;
	size_t space, split;
	int count, i;

	if (uk_refcount_read(&original->sg_refs) > 1)
		return -EINVAL;

	/* Figure out how big of a sglist '*head' has to hold. */
	count = 0;
	space = 0;
	split = 0;
	for (i = 0; i < original->sg_nseg; i++) {
		space += original->sg_segs[i].ss_len;
		count++;
		if (space >= length) {
			/*
			 * If 'length' falls in the middle of a
			 * scatter/gather list entry, then 'split'
			 * holds how much of that entry will remain in
			 * 'original'.
			 */
			split = space - length;
			break;
		}
	}

	/* Nothing to do, so leave head empty. */
	if (count == 0)
		return 0;

	if (*head == NULL) {
		sg = uk_sglist_alloc(count);
		if (sg == NULL)
			return -ENOMEM;
		*head = sg;
	} else {
		sg = *head;
		if (sg->sg_maxseg < count)
			return -EFBIG;
		if (sg->sg_nseg != 0)
			return -EINVAL;
	}

	/* Copy 'count' entries to 'sg' from 'original'. */
	memmove(sg->sg_segs, original->sg_segs,
			count * sizeof(struct uk_sglist_seg));
	sg->sg_nseg = count;

	/*
	 * If we had to split a list entry, fixup the last entry in
	 * 'sg' and the new first entry in 'original'.  We also
	 * decrement 'count' by 1 since we will only be removing
	 * 'count - 1' segments from 'original' now.
	 */
	if (split != 0) {
		count--;
		sg->sg_segs[count].ss_len -= split;
		original->sg_segs[count].ss_paddr =
		    sg->sg_segs[count].ss_paddr + split;
		original->sg_segs[count].ss_len = split;
	}

	/* Trim 'count' entries from the front of 'original'. */
	original->sg_nseg -= count;
	memmove(original->sg_segs, original->sg_segs + count,
			count * sizeof(struct uk_sglist_seg));
	return 0;
}

int uk_sglist_slice(struct uk_sglist *original, struct uk_sglist **slice,
			size_t offset, size_t length)
{
	struct uk_sglist *sg;
	size_t space, end, foffs, loffs;
	int count, i, fseg;

	/* Nothing to do. */
	if (length == 0)
		return 0;

	/* Figure out how many segments '*slice' needs to have. */
	end = offset + length;
	space = 0;
	count = 0;
	fseg = 0;
	foffs = loffs = 0;
	for (i = 0; i < original->sg_nseg; i++) {
		space += original->sg_segs[i].ss_len;
		if (space > offset) {
			/*
			 * When we hit the first segment, store its index
			 * in 'fseg' and the offset into the first segment
			 * of 'offset' in 'foffs'.
			 */
			if (count == 0) {
				fseg = i;
				foffs = offset - (space -
				    original->sg_segs[i].ss_len);
				debug("sglist_slice: foffs = %08lx",
						foffs);
			}
			count++;

			/*
			 * When we hit the last segment, break out of
			 * the loop.  Store the amount of extra space
			 * at the end of this segment in 'loffs'.
			 */
			if (space >= end) {
				loffs = space - end;
				debug("sglist_slice: loffs = %08lx",
						loffs);
				break;
			}
		}
	}

	/* If we never hit 'end', then 'length' ran off the end, so fail. */
	if (space < end)
		return -EINVAL;

	if (*slice == NULL) {
		sg = uk_sglist_alloc(count);
		if (sg == NULL)
			return -ENOMEM;
		*slice = sg;
	} else {
		sg = *slice;
		if (sg->sg_maxseg < count)
			return -EFBIG;
		if (sg->sg_nseg != 0)
			return -EINVAL;
	}

	/*
	 * Copy over 'count' segments from 'original' starting at
	 * 'fseg' to 'sg'.
	 */
	memmove(sg->sg_segs, original->sg_segs + fseg,
	    count * sizeof(struct uk_sglist_seg));
	sg->sg_nseg = count;

	/* Fixup first and last segments if needed. */
	if (foffs != 0) {
		sg->sg_segs[0].ss_paddr += foffs;
		sg->sg_segs[0].ss_len -= foffs;
		debug("sglist_slice seg[0]: %16lx:%16lx",
		    (long)sg->sg_segs[0].ss_paddr, sg->sg_segs[0].ss_len);
	}
	if (loffs != 0) {
		sg->sg_segs[count - 1].ss_len -= loffs;
		debug("sglist_slice seg[%d]: len %08x",
			count - 1, (uint32_t)sg->sg_segs[count - 1].ss_len);
	}
	return 0;
}

int uk_sglist_append_netbuf(struct uk_sglist *sg, struct uk_netbuf *netbuf)
{
	struct sgsave save;
	struct uk_netbuf *nb;
	int error;

	if (sg->sg_maxseg == 0)
		return -EINVAL;

	error = 0;
	SGLIST_SAVE(sg, save);
	UK_NETBUF_CHAIN_FOREACH(nb, netbuf) {
		if (likely(nb->len > 0)) {
			error = uk_sglist_append(sg, nb->data, nb->len);
			if (unlikely(error)) {
				SGLIST_RESTORE(sg, save);
				return error;
			}
		}
	}
	return 0;
}
