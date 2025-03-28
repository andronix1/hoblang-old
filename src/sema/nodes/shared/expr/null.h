#pragma once

#include "core/location.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/expr.h"

SemaValue *sema_module_analyze_expr_null(SemaModule *sema, FileLocation loc, SemaExprCtx ctx);
