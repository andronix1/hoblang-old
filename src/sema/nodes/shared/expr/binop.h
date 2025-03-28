#pragma once

#include "ast/shared/expr/binop.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"

SemaValue *sema_module_analyze_expr_binop(SemaModule *sema, AstExprBinop *binop);
