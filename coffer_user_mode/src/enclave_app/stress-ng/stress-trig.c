/*
 * Copyright (C) 2023      Colin Ian King.
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
#include "core-pragma.h"
#include "core-put.h"
#include "core-target-clones.h"

#define PI			(3.14159265358979323846264338327950288419716939937511L)
#define TANSUM			(-710.4128636743199902703338466380955651402473L)
#define STRESS_TRIG_LOOPS	(10000)

typedef struct {
	const char *name;
	bool (*trig_func)(stress_args_t *args);
} stress_trig_method_t;

static const stress_help_t help[] = {
	{ NULL,	"trig N",	 "start N workers exercising trigonometric functions" },
	{ NULL,	"trig-ops N",	 "stop after N trig bogo trigonometric operations" },
	{ NULL, "trig-method M", "select trigonometric function to exercise" },
	{ NULL,	NULL,		 NULL }
};

static bool OPTIMIZE3 TARGET_CLONES stress_trig_cos(stress_args_t *args)
{
	double sumcos = 0.0;
	double theta = 0.0;
	double dtheta = (PI * 2.0) / (double)STRESS_TRIG_LOOPS;
	double precision = 1E-7;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumcos += shim_cos(theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabs(sumcos - (double)0.0) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_cosf(stress_args_t *args)
{
	double sumcos = 0.0;
	double theta = 0.0;
	double dtheta = (PI * 2.0) / (float)STRESS_TRIG_LOOPS;
	double precision = 1E-4;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumcos += (double)shim_cosf((float)theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabs(sumcos - (float)0.0) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_cosl(stress_args_t *args)
{
	long double sumcos = 0.0L;
	long double theta = 0.0L;
	long double dtheta = (PI * 2.0L) / (long double)STRESS_TRIG_LOOPS;
	long double precision;
	int i;

	switch (sizeof(precision)) {
	case 16:
		precision = 1E-12;
		break;
	case 12:
		precision = 1E-8;
		break;
	default:
		precision = 1E-7;
		break;
	}

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumcos += shim_cosl(theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabsl(sumcos - (long double)0.0) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_sin(stress_args_t *args)
{
	double sumsin = 0.0;
	double theta = 0.0;
	double dtheta = (PI * 2.0) / (double)STRESS_TRIG_LOOPS;
	double precision = 1E-7;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumsin += shim_sin(theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabs(sumsin - (double)0.0) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_sinf(stress_args_t *args)
{
	double sumsin = 0.0;
	double theta = 0.0;
	double dtheta = (PI * 2.0) / (float)STRESS_TRIG_LOOPS;
	double precision = 1E-4;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumsin += (double)shim_sinf((float)theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabs(sumsin - (float)0.0) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_sinl(stress_args_t *args)
{
	long double sumsin = 0.0L;
	long double theta = 0.0L;
	long double dtheta = (PI * 2.0L) / (long double)STRESS_TRIG_LOOPS;
	long double precision;
	int i;

	switch (sizeof(precision)) {
	case 16:
		precision = 1E-12;
		break;
	case 12:
		precision = 1E-8;
		break;
	default:
		precision = 1E-7;
		break;
	}

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumsin += shim_sinl(theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabsl(sumsin - (long double)0.0) > precision;
}

#if defined(HAVE_SINCOS)
static bool OPTIMIZE3 TARGET_CLONES stress_trig_sincos(stress_args_t *args)
{
	double sumsin = 0.0, sumcos = 0.0;
	double theta = 0.0;
	double dtheta = (PI * 2.0) / (double)STRESS_TRIG_LOOPS;
	double precision = 1E-7;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		double c, s;

		shim_sincos(theta, &s, &c);
		sumsin += s;
		sumcos += c;
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return (shim_fabs(sumsin - (double)0.0) > precision) ||
	       (shim_fabs(sumcos - (double)0.0) > precision);
}
#endif

#if defined(HAVE_SINCOSF)
static bool OPTIMIZE3 TARGET_CLONES stress_trig_sincosf(stress_args_t *args)
{
	double sumsin = 0.0, sumcos = 0.0;
	double theta = 0.0;
	double dtheta = (PI * 2.0) / (float)STRESS_TRIG_LOOPS;
	double precision = 1E-4;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		float c, s;

		shim_sincosf((float)theta, &s, &c);
		sumsin += s;
		sumcos += c;
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return (shim_fabs(sumsin - (float)0.0) > precision) ||
	       (shim_fabs(sumcos - (float)0.0) > precision);
}
#endif

#if defined(HAVE_SINCOSL)
static bool OPTIMIZE3 TARGET_CLONES stress_trig_sincosl(stress_args_t *args)
{
	long double sumsin = 0.0, sumcos = 0.0;
	long double theta = 0.0L;
	long double dtheta = (PI * 2.0L) / (long double)STRESS_TRIG_LOOPS;
	long double precision;
	int i;

	switch (sizeof(precision)) {
	case 16:
		precision = 1E-12;
		break;
	case 12:
		precision = 1E-8;
		break;
	default:
		precision = 1E-7;
		break;
	}

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		long double s, c;

		shim_sincosl(theta, &s, &c);
		sumsin += s;
		sumcos += c;
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return (shim_fabsl(sumsin - (long double)0.0) > precision) ||
	       (shim_fabsl(sumcos - (long double)0.0) > precision);
}
#endif

static bool OPTIMIZE3 TARGET_CLONES stress_trig_tan(stress_args_t *args)
{
	double sumtan = 0.0;
	double theta = 3.0;
	double dtheta = ((double)PI - theta) / (double)STRESS_TRIG_LOOPS;
	double precision = 1E-7;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumtan += shim_tan(theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabs(sumtan - (double)TANSUM) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_tanf(stress_args_t *args)
{
	double sumtan = 0.0;
	double theta = 3.0;
	double dtheta = ((double)PI - theta) / (double)STRESS_TRIG_LOOPS;
	double precision = 1E-5;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumtan += shim_tanf((float)theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabs(sumtan - (double)TANSUM) > precision;
}

static bool OPTIMIZE3 TARGET_CLONES stress_trig_tanl(stress_args_t *args)
{
	long double sumtan = 0.0;
	long double theta = 3.0;
	long double dtheta = ((long double)PI - theta) / (long double)STRESS_TRIG_LOOPS;
	long double precision = 1E-7;
	int i;

PRAGMA_UNROLL_N(8)
	for (i = 0; i < STRESS_TRIG_LOOPS; i++) {
		sumtan += shim_tanl(theta);
		theta += dtheta;
	}
	stress_bogo_inc(args);
	return shim_fabsl(sumtan - (long double)TANSUM) > precision;
}

static bool stress_trig_all(stress_args_t *args);

static const stress_trig_method_t stress_trig_methods[] = {
	{ "all",	stress_trig_all },
	{ "cos",	stress_trig_cos	},
	{ "cosf",	stress_trig_cosf },
	{ "cosl",	stress_trig_cosl },
	{ "sin",	stress_trig_sin	},
	{ "sinf",	stress_trig_sinf },
	{ "sinl",	stress_trig_sinl },
#if defined(HAVE_SINCOS)
	{ "sincos",	stress_trig_sincos },
#endif
#if defined(HAVE_SINCOSF)
	{ "sincosf",	stress_trig_sincosf },
#endif
#if defined(HAVE_SINCOSL)
	{ "sincosl",	stress_trig_sincosl },
#endif
	{ "tan",	stress_trig_tan },
	{ "tanf",	stress_trig_tanf },
	{ "tanl",	stress_trig_tanl },
};

stress_metrics_t stress_trig_metrics[SIZEOF_ARRAY(stress_trig_methods)];

static int stress_set_trig_method(const char *opt)
{
	size_t i;

	for (i = 0; i < SIZEOF_ARRAY(stress_trig_methods); i++) {
		if (strcmp(opt, stress_trig_methods[i].name) == 0)
			return stress_set_setting("trig-method", TYPE_ID_SIZE_T, &i);
	}

	(void)fprintf(stderr, "trig-method must be one of:");
	for (i = 0; i < SIZEOF_ARRAY(stress_trig_methods); i++) {
		(void)fprintf(stderr, " %s", stress_trig_methods[i].name);
	}
	(void)fprintf(stderr, "\n");
	return -1;
}

static bool stess_trig_exercise(stress_args_t *args, const size_t index)
{
	bool ret;
	const double t = stress_time_now();

	ret = stress_trig_methods[index].trig_func(args);
	stress_trig_metrics[index].duration += (stress_time_now() - t);
	stress_trig_metrics[index].count += 1.0;
	if (ret) {
		pr_fail("trig: %s does not match expected checksum\n",
			stress_trig_methods[index].name);
	}
	return ret;
}

static bool stress_trig_all(stress_args_t *args)
{
	size_t i;
	bool ret = false;

	for (i = 1; i < SIZEOF_ARRAY(stress_trig_methods); i++) {
		ret |= stess_trig_exercise(args, i);
	}
	return ret;
}

/*
 * stress_trig()
 *	stress system by various trig function calls
 */
static int stress_trig(stress_args_t *args)
{
	size_t i, j;
	size_t trig_method = 0;

	(void)stress_get_setting("trig-method", &trig_method);

	for (i = 0; i < SIZEOF_ARRAY(stress_trig_metrics); i++) {
		stress_trig_metrics[i].duration = 0.0;
		stress_trig_metrics[i].count = 0.0;
	}

	stress_set_proc_state(args->name, STRESS_STATE_RUN);

	do {
		stess_trig_exercise(args, trig_method);
	} while (stress_continue(args));

	stress_set_proc_state(args->name, STRESS_STATE_DEINIT);

	for (i = 1, j = 0; i < SIZEOF_ARRAY(stress_trig_metrics); i++) {
		if (stress_trig_metrics[i].duration > 0.0) {
			char buf[80];
			const double rate = (double)STRESS_TRIG_LOOPS *
				stress_trig_metrics[i].count / stress_trig_metrics[i].duration;

			(void)snprintf(buf, sizeof(buf), "%s ops per second", stress_trig_methods[i].name);
			stress_metrics_set(args, j, buf,
				rate, STRESS_HARMONIC_MEAN);
			j++;
		}
	}
	return EXIT_SUCCESS;
}

static const stress_opt_set_func_t opt_set_funcs[] = {
	{ OPT_trig_method,	stress_set_trig_method },
	{ 0,			NULL },
};

stressor_info_t stress_trig_info = {
	.stressor = stress_trig,
	.class = CLASS_CPU,
	.opt_set_funcs = opt_set_funcs,
	.verify = VERIFY_ALWAYS,
	.help = help
};
