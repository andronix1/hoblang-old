#pragma once

#include "ast/interface/expr.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"

SemaValue *sema_module_analyze_expr_ret_on_null(SemaModule *module, AstExpr *inner);
