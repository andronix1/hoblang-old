#pragma once

#include "sema/interface/module.h"
#include "sema/interface/value.h"
#include "ast/shared/expr/call.h"

SemaValue *sema_module_analyze_expr_call(SemaModule *sema, AstExprCall *call);
