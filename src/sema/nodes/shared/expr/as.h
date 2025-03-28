#pragma once

#include "ast/shared/expr/as.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/expr.h"

SemaValue *sema_module_analyze_expr_as(SemaModule *sema, AstExprAs *as, SemaExprCtx ctx);
