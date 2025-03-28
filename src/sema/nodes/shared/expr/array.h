#pragma once

#include "core/location.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "ast/shared/expr/array.h"
#include "sema/nodes/shared/expr.h"

SemaValue *sema_module_analyze_expr_array(SemaModule *module, FileLocation loc, AstExprArray *array, SemaExprCtx ctx);
