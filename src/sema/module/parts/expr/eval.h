#pragma once

#include <stdint.h>
#include "ast/api/expr.h"
#include "sema/module.h"

uint64_t sema_eval_int_expr(SemaModule *sema, AstExpr *expr);
