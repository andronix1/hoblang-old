#pragma once

#include <stdlib.h>
#include <valgrind/valgrind.h>
#include "log.h"

#undef assert
#define assert(ok, fmt, ...) \
	do { \
		if (!(ok)) { \
			hob_log(LOGF, fmt, ##__VA_ARGS__); \
			VALGRIND_PRINTF_BACKTRACE("\n"); \
			exit(1); \
		} \
	} while (0)
