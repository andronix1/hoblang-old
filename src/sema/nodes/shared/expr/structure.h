#pragma once

#include "ast/shared/expr/structure.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"

SemaValue *sema_module_analyze_expr_structure(SemaModule *module, AstExprStructure *structure);
