#pragma once

#include <stdbool.h>
#include "ast/api/expr.h"
#include "ast/api/body.h"
#include "sema/module/loop/loop.h"
#include "core/slice.h"
#include "core/location.h"

typedef struct {
    FileLocation loc;
	AstExpr *expr;
	AstBody *body;
    bool is_named;
    Slice name;

    SemaLoop *loop;
} AstWhile;
