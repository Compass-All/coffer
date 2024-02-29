/*
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
#include "core-builtin.h"
#include "core-out-of-memory.h"

#include <sched.h>

#if defined(HAVE_SCHED_SETAFFINITY) &&					     \
    (defined(_POSIX_PRIORITY_SCHEDULING) || defined(__linux__)) &&	     \
    (defined(SCHED_OTHER) || defined(SCHED_BATCH) || defined(SCHED_IDLE)) && \
    !defined(__OpenBSD__) &&						     \
    !defined(__minix__) &&						     \
    !defined(__APPLE__)
#define HAVE_SCHEDULING
#endif

#define DEFAULT_CHILDREN		(8)

#define RACE_SCHED_METHOD_ALL		(0)
#define RACE_SCHED_METHOD_NEXT		(1)
#define RACE_SCHED_METHOD_PREV		(2)
#define RACE_SCHED_METHOD_RAND		(3)
#define RACE_SCHED_METHOD_RANDINC	(4)
#define RACE_SCHED_METHOD_SYNCNEXT	(5)
#define RACE_SCHED_METHOD_SYNCPREV	(6)

typedef struct {
	const char *name;
	const int   method;
} stress_race_sched_method_t;

typedef struct stress_race_sched {
	struct stress_race_sched *next;
	pid_t	pid;
	int	cpu;
} stress_race_sched_child_t;

typedef struct {
	stress_race_sched_child_t *head;	/* Head of process list */
	stress_race_sched_child_t *tail;	/* Tail of process list */
	stress_race_sched_child_t *free;	/* List of free'd processes */
	uint32_t length;			/* Length of list */
} stress_race_sched_list_t;

static const stress_help_t help[] = {
	{ NULL,	"race-sched N",		"start N workers that race cpu affinity" },
	{ NULL,	"race-sched-ops N",	"stop after N bogo race operations" },
	{ NULL, "race-sched-method M",	"method M: all, rand, next, prev, yoyo, randinc" },
	{ NULL,	NULL,			NULL }
};

static const stress_race_sched_method_t stress_race_sched_methods[] = {
	{ "all",	RACE_SCHED_METHOD_ALL },
	{ "next",	RACE_SCHED_METHOD_NEXT },
	{ "prev",	RACE_SCHED_METHOD_PREV },
	{ "rand",	RACE_SCHED_METHOD_RAND },
	{ "randinc",	RACE_SCHED_METHOD_RANDINC },
	{ "syncnext",	RACE_SCHED_METHOD_SYNCNEXT },
	{ "syncprev",	RACE_SCHED_METHOD_SYNCPREV },
};

/*
 *  stress_set_race_sched_method()
 *	set the default race sched method
 */
static int stress_set_race_sched_method(const char *name)
{
	size_t i;

	for (i = 0; i < SIZEOF_ARRAY(stress_race_sched_methods); i++) {
		if (!strcmp(stress_race_sched_methods[i].name, name)) {
			stress_set_setting("race-sched-method", TYPE_ID_SIZE_T, &i);
			return 0;
		}
	}

	(void)fprintf(stderr, "race-sched-method must be one of:");
	for (i = 0; i < SIZEOF_ARRAY(stress_race_sched_methods); i++) {
		(void)fprintf(stderr, " %s", stress_race_sched_methods[i].name);
	}
	(void)fprintf(stderr, "\n");

	return -1;
}

static const stress_opt_set_func_t opt_set_funcs[] = {
	{ OPT_race_sched_method,	stress_set_race_sched_method },
	{ 0,				NULL },
};

#if defined(HAVE_SCHEDULING) &&		\
    defined(HAVE_SCHED_SETSCHEDULER)
static stress_race_sched_list_t children;

/*
 *  "Normal" non-realtime scheduling policies
 */
static const int normal_policies[] = {
#if defined(SCHED_OTHER)
		SCHED_OTHER,
#endif
#if defined(SCHED_BATCH)
		SCHED_BATCH,
#endif
#if defined(SCHED_IDLE)
		SCHED_IDLE,
#endif
};


static int stress_race_sched_method(const int cpu, const int max_cpus, size_t method_index)
{

	static size_t method_all_index = 1;
	int new_cpu = cpu;

again:
	switch (stress_race_sched_methods[method_index].method) {
	default:
	case RACE_SCHED_METHOD_ALL:
		method_index = method_all_index;
		method_all_index++;
		if (method_all_index > SIZEOF_ARRAY(stress_race_sched_methods))
			method_all_index = 1;
		goto again;
	case RACE_SCHED_METHOD_NEXT:
		new_cpu++;
		if (new_cpu >= max_cpus)
			new_cpu = 0;
		break;
	case RACE_SCHED_METHOD_PREV:
		new_cpu--;
		if (new_cpu < 0)
			new_cpu = max_cpus - 1;
		break;
	case RACE_SCHED_METHOD_RAND:
		new_cpu = (int)stress_mwc32modn((uint32_t)max_cpus);
		break;
	case RACE_SCHED_METHOD_RANDINC:
		new_cpu += (int)(stress_mwc8modn((uint32_t)max_cpus) & 0x3) + 1;
		new_cpu = (uint32_t)new_cpu % (uint32_t)max_cpus;
		break;
	case RACE_SCHED_METHOD_SYNCNEXT:
		/* Move every second */
		new_cpu = (uint32_t)rint(stress_time_now()) % (uint32_t)max_cpus;
		break;
	case RACE_SCHED_METHOD_SYNCPREV:
		/* Move every second */
		new_cpu = (~(uint32_t)rint(stress_time_now())) % (uint32_t)max_cpus;
		break;
	}
	return new_cpu;
}

static int stress_race_sched_setaffinity(
	stress_args_t *args,
	const pid_t pid,
	const int cpu)
{
	cpu_set_t cpu_set;
	int ret;

	CPU_ZERO(&cpu_set);
	CPU_SET(cpu, &cpu_set);
	ret = sched_setaffinity(pid, sizeof(cpu_set), &cpu_set);
	if (ret == 0) {
		CPU_ZERO(&cpu_set);
		ret = sched_getaffinity(pid, sizeof(cpu_set), &cpu_set);
		if ((ret < 0) && (errno != ESRCH)) {
			pr_fail("%s: sched_getaffinity failed on PID %jd, errno=%d (%s)\n",
				args->name, (intmax_t)pid, errno, strerror(errno));
			return ret;
		}
	}
	return 0;
}

static int stress_race_sched_setscheduler(
	stress_args_t *args,
	const pid_t pid)
{
	struct sched_param param;
	const uint32_t i = stress_mwc8modn((uint8_t)SIZEOF_ARRAY(normal_policies));
	int ret;

	(void)shim_memset(&param, 0, sizeof(param));
	param.sched_priority = 0;
	ret = sched_setscheduler(pid, normal_policies[i], &param);
	if (ret == 0) {
		ret = sched_getscheduler(pid);
		if ((ret < 0) && (errno != ESRCH)) {
			pr_fail("%s: sched_getscheduler failed on PID %jd, errno=%d (%s)\n",
				args->name, (intmax_t)pid, errno, strerror(errno));
			return ret;
		}
	}
	return 0;
}

static int stress_race_sched_exercise(
	stress_args_t *args,
	const int cpus,
	const size_t method_index)
{
	stress_race_sched_child_t *child;
	int i, rc = 0;

	for (i = 0; stress_continue_flag() && (i < 20); i++)  {
		for (child = children.head; child; child = child->next) {
			if (stress_mwc1()) {
				const int cpu = stress_race_sched_method(child->cpu, cpus, method_index);

				child->cpu = cpu;
				if (stress_race_sched_setaffinity(args, child->pid, cpu) < 0)
					rc = -1;
				if (stress_race_sched_setscheduler(args, child->pid) < 0)
					rc = -1;
			}
		}
	}
	return rc;
}

/*
 *  stress_race_sched_new()
 *	allocate a new child, add to end of list
 */
static stress_race_sched_child_t *stress_race_sched_new(void)
{
	stress_race_sched_child_t *new;

	if (children.free) {
		/* Pop an old one off the free list */
		new = children.free;
		children.free = new->next;
		new->next = NULL;
	} else {
		new = calloc(1, sizeof(*new));
		if (!new)
			return NULL;
	}

	if (children.head)
		children.tail->next = new;
	else
		children.head = new;

	children.tail = new;
	children.length++;

	return new;
}

/*
 *  stress_race_sched_head_remove
 *	reap a child and remove a child from head of list, put it onto
 *	the free child list
 */
static void stress_race_sched_head_remove(const int options)
{
	if (children.head) {
		int status, ret;
		stress_race_sched_child_t *head = children.head;

		ret = waitpid(children.head->pid, &status, options);
		if (ret >= 0) {
			if (children.tail == children.head) {
				children.tail = NULL;
				children.head = NULL;
			} else {
				children.head = head->next;
			}
			/* Shove it on the free list */
			head->next = children.free;
			children.free = head;
			children.length--;
		}
	}
}

/*
 *  stress_race_sched_free()
 *	free the children off the child free lists
 */
static void stress_race_sched_free(void)
{
	while (children.head) {
		stress_race_sched_child_t *next = children.head->next;

		free(children.head);
		children.head = next;
	}
	while (children.free) {
		stress_race_sched_child_t *next = children.free->next;

		free(children.free);
		children.free = next;
	}
}

static int stress_race_sched_child(stress_args_t *args, void *context)
{
	/* Child */
	uint32_t max_forks = 0;
	uint32_t children_max = DEFAULT_CHILDREN;
	int cpu = 0, cpus = (int)stress_get_processors_configured(), rc = EXIT_SUCCESS;
	size_t method_index = 0;
	const pid_t mypid = getpid();

	(void)stress_get_setting("race-sched-method", &method_index);

	(void)context;

	if (cpus < 1)
		cpus = 1;

	do {
		const bool low_mem_reap = ((g_opt_flags & OPT_FLAGS_OOM_AVOID) &&
					   stress_low_memory((size_t)(1 * MB)));
		const uint8_t rnd = stress_mwc8();

		cpu = stress_race_sched_method(cpu, cpus, method_index);
		if (stress_race_sched_setaffinity(args, mypid, cpu) < 0) {
			rc = EXIT_FAILURE;
			break;
		}

		if (!low_mem_reap && (children.length < children_max)) {
			stress_race_sched_child_t *child_info;

			child_info = stress_race_sched_new();
			if (!child_info)
				break;

			child_info->cpu = cpu;
			child_info->pid = fork();
			if (child_info->pid < 0) {
				/*
				 * Reached max forks or error
				 * (e.g. EPERM)? .. then reap
				 */
				if (stress_race_sched_exercise(args, cpus, method_index) < 0) {
					rc = EXIT_FAILURE;
					break;
				}
				stress_race_sched_head_remove(WNOHANG);
				continue;
			} else if (child_info->pid == 0) {
				/* child */
				const pid_t child_pid = getpid();

				if (rnd & 0x01)
					shim_sched_yield();
				if (rnd & 0x02) {
					if (stress_race_sched_setaffinity(args, child_pid, cpu) < 0) {
						rc = EXIT_FAILURE;
						break;
					}
				}
				if (rnd & 0x04) {
					if (stress_race_sched_setscheduler(args, child_pid) < 0) {
						rc = EXIT_FAILURE;
						break;
					}
				}
				if (rnd & 0x08) {
					if (stress_race_sched_exercise(args, cpus, method_index) < 0) {
						rc = EXIT_FAILURE;
						break;
					}
				}
				if (rnd & 0x10)
					shim_sched_yield();
				_exit(0);
			} else {
				/* parent */
				if (rnd & 0x20)
					shim_sched_yield();
				if (rnd & 0x40) {
					if (stress_race_sched_exercise(args, cpus, method_index) < 0) {
						rc = EXIT_FAILURE;
						break;
					}
				}
				if (rnd & 0x80)
					shim_sched_yield();
			}

			if (max_forks < children.length)
				max_forks = children.length;
			stress_bogo_inc(args);
		} else {
			if (rnd & 0x01) {
				if (stress_race_sched_exercise(args, cpus, method_index) < 0) {
					rc = EXIT_FAILURE;
					break;
				}
			}
			stress_race_sched_head_remove(WNOHANG);
			if (rnd & 0x02) {
				if (stress_race_sched_exercise(args, cpus, method_index) < 0) {
					rc = EXIT_FAILURE;
					break;
				}
			}
		}
	} while (stress_continue(args));

	/* And reap */
	while (children.head) {
		stress_race_sched_head_remove(0);
	}
	/* And free */
	stress_race_sched_free();

	return rc;
}

/*
 *  stress_race_sched()
 *	stress by cloning and exiting
 */
static int stress_race_sched(stress_args_t *args)
{
	int rc;

	stress_set_oom_adjustment(args, false);
	stress_set_proc_state(args->name, STRESS_STATE_RUN);
	rc = stress_oomable_child(args, NULL, stress_race_sched_child, STRESS_OOMABLE_DROP_CAP);
	stress_set_proc_state(args->name, STRESS_STATE_DEINIT);

	return rc;
}

stressor_info_t stress_race_sched_info = {
	.stressor = stress_race_sched,
	.class = CLASS_SCHEDULER | CLASS_OS,
	.opt_set_funcs = opt_set_funcs,
	.verify = VERIFY_ALWAYS,
	.help = help
};

#else

stressor_info_t stress_race_sched_info = {
        .stressor = stress_unimplemented,
	.class = CLASS_SCHEDULER | CLASS_OS,
	.opt_set_funcs = opt_set_funcs,
	.help = help,
	.verify = VERIFY_ALWAYS,
	.unimplemented_reason = "built without Linux scheduling or sched_setscheduler() system call"
};
#endif
