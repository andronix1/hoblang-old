#pragma once

#include <stdlib.h>
#include "log.h"

#undef assert
#define assert(ok, fmt, ...) \
	do { \
		if (!(ok)) { \
			hob_log(LOGE, "assertion error(" __FILE__ "): " fmt, ##__VA_ARGS__); \
			exit(1); \
		} \
	} while (0)
