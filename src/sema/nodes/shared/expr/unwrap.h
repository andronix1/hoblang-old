#pragma once

#include "ast/shared/expr/unwrap.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"

SemaValue *sema_module_analyze_expr_unwrap(SemaModule *module, AstExprUnwrap *array);
