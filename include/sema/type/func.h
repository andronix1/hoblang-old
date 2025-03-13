#pragma once

#include "type.h"

typedef struct SemaFunction {
	SemaType **args;
	SemaType *returning;
} SemaFunction;
