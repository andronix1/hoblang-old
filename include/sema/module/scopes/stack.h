#pragma once

#include "sema/module/loop/loop.h"
#include "scope.h"
#include "sema/type/private.h"

typedef struct SemaScopeStack {
    SemaScope *scopes;
	SemaLoop **loops;
	SemaType *returning;
} SemaScopeStack;
