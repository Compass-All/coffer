/*
 * Copyright (C) 2013-2021 Canonical, Ltd.
 * Copyright (C) 2022-2023 Colin Ian King.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "stress-ng.h"
#include "core-shim.h"
#include "core-sort.h"

#if defined(HAVE_SEARCH_H)
#include <search.h>
#endif

typedef void * (*bsearch_func_t)(const void *key, const void *base, size_t nmemb, size_t size,
			       int (*compare)(const void *p1, const void *p2));

typedef struct {
	const char *name;
	const bsearch_func_t bsearch_func;
} stress_bsearch_method_t;

#define MIN_BSEARCH_SIZE	(1 * KB)
#define MAX_BSEARCH_SIZE	(4 * MB)
#define DEFAULT_BSEARCH_SIZE	(64 * KB)

static const stress_help_t help[] = {
	{ NULL,	"bsearch N",	  	"start N workers that exercise a binary search" },
	{ NULL,	"bsearch-method M",	"select bsearch method [ bsearch-libc | bsearch-nonlibc ]" },
	{ NULL,	"bsearch-ops N",  	"stop after N binary search bogo operations" },
	{ NULL,	"bsearch-size N", 	"number of 32 bit integers to bsearch" },
	{ NULL,	NULL,			NULL }
};

/*
 *  stress_set_bsearch_size()
 *	set bsearch size from given option string
 */
static int stress_set_bsearch_size(const char *opt)
{
	uint64_t bsearch_size;

	bsearch_size = stress_get_uint64(opt);
	stress_check_range("bsearch-size", bsearch_size,
		MIN_BSEARCH_SIZE, MAX_BSEARCH_SIZE);
	return stress_set_setting("bsearch-size", TYPE_ID_UINT64, &bsearch_size);
}

static void OPTIMIZE3 * bsearch_nonlibc(
	const void *key,
	const void *base,
	size_t nmemb,
	size_t size,
	int (*compare)(const void *p1, const void *p2))
{
	register size_t lower = 0;
	register size_t upper = nmemb;

	while (LIKELY(lower < upper)) {
		register size_t index = (lower + upper) >> 1;
		register const void *ptr = (const char *)base + (index * size);
		register int cmp = compare(key, ptr);

		if (cmp < 0) {
			upper = index;
		} else if (cmp > 0) {
			lower = index + 1;
		} else {
			return shim_unconstify_ptr(ptr);
		}
	}
	return NULL;
}

static const stress_bsearch_method_t stress_bsearch_methods[] = {
#if defined(HAVE_BSEARCH)
	{ "bsearch-libc",	bsearch },
#endif
	{ "bsearch-nonlibc",	bsearch_nonlibc },
};

static int stress_set_bsearch_method(const char *opt)
{
	size_t i;

	for (i = 0; i < SIZEOF_ARRAY(stress_bsearch_methods); i++) {
		if (strcmp(opt, stress_bsearch_methods[i].name) == 0) {
			stress_set_setting("bsearch-method", TYPE_ID_SIZE_T, &i);
			return 0;
		}
	}

	(void)fprintf(stderr, "bsearch-method must be one of:");
	for (i = 0; i < SIZEOF_ARRAY(stress_bsearch_methods); i++) {
		(void)fprintf(stderr, " %s", stress_bsearch_methods[i].name);
	}
	(void)fprintf(stderr, "\n");
	return -1;
}

/*
 *  stress_bsearch()
 *	stress bsearch
 */
static int OPTIMIZE3 stress_bsearch(stress_args_t *args)
{
	int32_t *data, *ptr;
	size_t n, n8, i, bsearch_method = 0, data_size;
	uint64_t bsearch_size = DEFAULT_BSEARCH_SIZE;
	double rate, duration = 0.0, count = 0.0, sorted = 0.0;
	bsearch_func_t bsearch_func;

	(void)stress_get_setting("bsearch-method", &bsearch_method);
	bsearch_func = stress_bsearch_methods[bsearch_method].bsearch_func;

	if (!stress_get_setting("bsearch-size", &bsearch_size)) {
		if (g_opt_flags & OPT_FLAGS_MAXIMIZE)
			bsearch_size = MAX_BSEARCH_SIZE;
		if (g_opt_flags & OPT_FLAGS_MINIMIZE)
			bsearch_size = MIN_BSEARCH_SIZE;
	}
	n = (size_t)bsearch_size;
	n8 = (n + 7) & ~7UL;
	data_size = n8 * sizeof(*data);

	/* allocate in multiples of 8 */
	data = (int32_t *)stress_mmap_populate(NULL,
				data_size, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (data == MAP_FAILED) {
		pr_inf_skip("%s: mmap of %zu bytes failed, errno=%d (%s), skipping stressor\n",
			args->name, data_size, errno, strerror(errno));
		return EXIT_NO_RESOURCE;
	}

	stress_set_proc_state(args->name, STRESS_STATE_RUN);

	do {
		double t;

		stress_sort_data_int32_init(data, n);
		stress_sort_compare_reset();
		t = stress_time_now();
		for (ptr = data, i = 0; i < n; i++, ptr++) {
			int32_t *result;

			result = bsearch_func(ptr, data, n, sizeof(*ptr), stress_sort_cmp_fwd_int32);
			if (g_opt_flags & OPT_FLAGS_VERIFY) {
				if (result == NULL)
					pr_fail("%s: element %zu could not be found\n",
						args->name, i);
				else if (*result != *ptr)
					pr_fail("%s: element %zu "
						"found %" PRIu32
						", expecting %" PRIu32 "\n",
						args->name, i, *result, *ptr);
			}
		}
		duration += stress_time_now() - t;
		count += (double)stress_sort_compare_get();
		sorted += (double)i;
		stress_bogo_inc(args);
	} while (stress_continue(args));

	stress_set_proc_state(args->name, STRESS_STATE_DEINIT);

	rate = (duration > 0.0) ? count / duration : 0.0;
	stress_metrics_set(args, 0, "bsearch comparisons per sec",
		rate, STRESS_HARMONIC_MEAN);
	stress_metrics_set(args, 1, "bsearch comparisons per item",
		count / sorted, STRESS_HARMONIC_MEAN);

	(void)munmap((void *)data, data_size);
	return EXIT_SUCCESS;
}

static const stress_opt_set_func_t opt_set_funcs[] = {
	{ OPT_bsearch_size,	stress_set_bsearch_size },
	{ OPT_bsearch_method,	stress_set_bsearch_method },
	{ 0,			NULL }
};

stressor_info_t stress_bsearch_info = {
	.stressor = stress_bsearch,
	.class = CLASS_CPU_CACHE | CLASS_CPU | CLASS_MEMORY,
	.opt_set_funcs = opt_set_funcs,
	.verify = VERIFY_OPTIONAL,
	.help = help
};
