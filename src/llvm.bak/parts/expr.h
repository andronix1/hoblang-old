#pragma once

#include <llvm-c/Core.h>
#include <stdbool.h>
#include "llvm/llvm.h"
#include "ast/api/expr.h"

LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr, bool load);
