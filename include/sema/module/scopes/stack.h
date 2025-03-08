#pragma once

#include "sema/module/loop/loop.h"
#include "scope.h"

typedef struct SemaScopeStack {
    SemaScope *scopes;
	SemaLoop **loops;
} SemaScopeStack;
