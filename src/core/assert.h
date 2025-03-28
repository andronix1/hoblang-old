#pragma once

#include <stdlib.h>
#include <valgrind/valgrind.h>
#include "core/log.h"

#undef assert
#define assert(ok, fmt, ...) \
	do { \
		if (!(ok)) { \
			hob_log(LOGF, "assertion failed({cstr}:{int}): " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
			VALGRIND_PRINTF_BACKTRACE("\n"); \
			exit(1); \
		} \
	} while (0)

