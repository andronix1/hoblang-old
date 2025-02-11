#pragma once

#include "ast/expr.h"
#include "../parts.h"
#include "../path.h"

uint64_t sema_eval_int_expr(SemaModule *sema, AstExpr *expr);
