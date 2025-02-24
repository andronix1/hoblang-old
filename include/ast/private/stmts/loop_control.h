#pragma once

#include <stdbool.h>
#include "core/slice.h"
#include "sema/module/loop/loop.h"
#include "ast/api/defer.h"

typedef struct {
    bool is_named;
    Slice loop_name;

	AstDefer **defers;

    SemaLoop *loop;
} AstStmtLoopControl;
