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
#include "core-builtin.h"

#if defined(HAVE_CRYPT_H)
#include <crypt.h>
#endif

static const stress_help_t help[] = {
	{ NULL,	"crypt N",	"start N workers performing password encryption" },
	{ NULL,	"crypt-ops N",	"stop after N bogo crypt operations" },
	{ NULL,	NULL,		NULL }
};

#if defined(HAVE_LIB_CRYPT) &&	\
    defined(HAVE_CRYPT_H)

typedef struct {
	const char *prefix;
	const size_t prefix_len;
	const char *method;
} crypt_method_t;

static const crypt_method_t crypt_methods[] = {
	{ "$1$",	3,	"MD5" },
	{ "$3$",	3,	"NT" },
	{ "$sha1",	5,	"SHA-1" },
	{ "$5$",	3,	"SHA-256" },
	{ "$6$",	3,	"SHA-512" },
	{ "$7$",	3,	"scrypt" },
	{ "$md5",	5,	"SunMD5" },
	{ "$y$",	3,	"yescrypt" },
};

static stress_metrics_t *crypt_metrics;

/*
 *  stress_crypt_id()
 *	crypt a password with given seed and id
 */
static int stress_crypt_id(
	stress_args_t *args,
	const size_t i,
#if defined(HAVE_CRYPT_R)
	struct crypt_data *data
#else
	const char *phrase,
	const char *setting
#endif
	)
{
	char *encrypted;
	double t1, t2;
	errno = 0;

#if defined(HAVE_CRYPT_R)
	t1 = stress_time_now();
	encrypted = crypt_r(data->input, data->setting, data);
	t2 = stress_time_now();
#else
	t1 = stress_time_now();
	encrypted = crypt(phrase, setting);
	t2 = stress_time_now();
#endif
	if (UNLIKELY(!encrypted)) {
		switch (errno) {
		case 0:
			break;
		case EINVAL:
			break;
#if defined(ENOENT)
		case ENOENT:
			break;
#endif
#if defined(ENOSYS)
		case ENOSYS:
			break;
#endif
#if defined(EOPNOTSUPP)
		case EOPNOTSUPP:
#endif
			break;
		default:
			pr_fail("%s: cannot encrypt with %s, errno=%d (%s)\n",
				args->name, crypt_methods[i].method, errno, strerror(errno));
			return -1;
		}
	} else {
		crypt_metrics[i].duration += (t2 - t1);
		crypt_metrics[i].count += 1.0;
	}
	return 0;
}

/*
 *  stress_crypt()
 *	stress libc crypt
 */
static int stress_crypt(stress_args_t *args)
{
	register size_t i;

	crypt_metrics = calloc(SIZEOF_ARRAY(crypt_methods), sizeof(*crypt_metrics));
	if (!crypt_metrics) {
		pr_inf_skip("%s: cannot allocate crypt metrics "
			"array, skipping stressor\n",
			args->name);
		return EXIT_NO_RESOURCE;
	}
	for (i = 0; i < SIZEOF_ARRAY(crypt_methods); i++) {
		crypt_metrics[i].duration = 0.0;
		crypt_metrics[i].count = 0.0;
	}

	stress_set_proc_state(args->name, STRESS_STATE_RUN);

	do {
		static const char seedchars[] ALIGN64 =
			"./0123456789ABCDEFGHIJKLMNOPQRST"
			"UVWXYZabcdefghijklmnopqrstuvwxyz";
#if defined(HAVE_CRYPT_R)
		static struct crypt_data data;
		char *const phrase = data.input;
		char *const setting = data.setting;
		const size_t setting_len = sizeof(data.setting);
#else
		char phrase[16];
		char setting[12];
		const size_t setting_len = sizeof(setting);
#endif
		char orig_setting[12];
		uint64_t seed[2];
		const crypt_method_t *cm = crypt_methods;
		size_t failed = 0;

		seed[0] = stress_mwc64();
		seed[1] = stress_mwc64();

		for (i = 0; i < 8; i++)
			orig_setting[i + 3] = seedchars[(seed[i / 5] >> (i % 5) * 6) & 0x3f];
		for (i = 0; i < sizeof(phrase) - 1; i++)
			phrase[i] = seedchars[stress_mwc32modn((uint32_t)sizeof(seedchars))];
		phrase[i] = '\0';

		for (i = 0; stress_continue(args) && (i < SIZEOF_ARRAY(crypt_methods)); i++, cm++) {
			int ret;

			(void)shim_strlcpy(setting, orig_setting, setting_len);
			(void)shim_memcpy(setting, cm->prefix, cm->prefix_len);
#if defined (HAVE_CRYPT_R)
			data.initialized = 0;
#endif

			ret = stress_crypt_id(args, i,
#if defined (HAVE_CRYPT_R)
					      &data);
#else
					      phrase, setting);
#endif
			if (UNLIKELY(ret < 0))
				failed++;
			else
				stress_bogo_inc(args);
		}
		if (failed)
			break;
	} while (stress_continue(args));

	stress_set_proc_state(args->name, STRESS_STATE_DEINIT);

	for (i = 0; i < SIZEOF_ARRAY(crypt_methods); i++) {
		char str[40];
		const double duration = crypt_metrics[i].duration;
		const double rate = duration > 0 ? crypt_metrics[i].count / duration : 0.0;

		(void)snprintf(str, sizeof(str), "%s encrypts per sec", crypt_methods[i].method);
		stress_metrics_set(args, i, str, rate, STRESS_HARMONIC_MEAN);
	}

	free(crypt_metrics);

	return EXIT_SUCCESS;
}

stressor_info_t stress_crypt_info = {
	.stressor = stress_crypt,
	.class = CLASS_CPU,
	.verify = VERIFY_ALWAYS,
	.help = help
};
#else
stressor_info_t stress_crypt_info = {
	.stressor = stress_unimplemented,
	.class = CLASS_CPU,
	.verify = VERIFY_ALWAYS,
	.help = help,
	.unimplemented_reason = "built without gcrypt library"
};
#endif
