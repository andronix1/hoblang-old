#pragma once

#include "ast/shared/expr/anon_func.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/expr.h"

SemaValue *sema_module_analyze_expr_anon_func(SemaModule *module, FileLocation loc, AstExprAnonFunc *func, SemaExprCtx ctx);
