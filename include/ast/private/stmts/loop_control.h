#pragma once

#include <stdbool.h>
#include "core/location.h"
#include "core/slice.h"
#include "sema/module/loop/loop.h"
#include "ast/api/defer.h"

typedef struct {
    FileLocation loc, name_loc;
    bool is_named;
    Slice loop_name;

	AstDefer **defers;

    SemaLoop *loop;
} AstStmtLoopControl;
