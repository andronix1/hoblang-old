#pragma once

#include "private.h"
#include "tokens.h"
#include "core/chars.h"

typedef enum {
	LEX_PART_OK = 0,
	LEX_PART_ERR,
	LEX_PART_MISSMATCH
} LexPartErr;
