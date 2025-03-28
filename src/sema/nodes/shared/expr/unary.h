#pragma once

#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "ast/shared/expr/unary.h"

SemaValue *sema_module_analyze_expr_unary(SemaModule *module, AstExprUnary *inner);
