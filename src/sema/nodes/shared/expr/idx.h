#pragma once

#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "ast/shared/expr/idx.h"

SemaValue *sema_module_analyze_expr_idx(SemaModule *sema, AstExprIdx *idx);
