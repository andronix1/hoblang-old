#pragma once

#include <stdbool.h>
#include "core/slice.h"
#include "sema/module/loop/loop.h"

typedef struct {
    bool is_named;
    Slice loop_name;

    SemaLoop *loop;
} AstStmtLoopControl;
