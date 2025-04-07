#pragma once

#include <stdlib.h>
#include "core/log.h"

void print_backtrace();

#undef assert
#define assert(ok, fmt, ...) \
	do { \
		if (!(ok)) { \
			hob_log(LOGF, "assertion failed({cstr}:{int}): " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
            print_backtrace(); \
			exit(1); \
		} \
	} while (0)

