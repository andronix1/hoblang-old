#pragma once

#include <stdlib.h>
#include "log.h"

#undef assert
#define assert(ok, fmt, ...) \
	do { \
		if (!(ok)) { \
			hob_log(LOGE, fmt, ##__VA_ARGS__); \
			exit(1); \
		} \
	} while (0)
