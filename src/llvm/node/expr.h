#pragma once

#include "ast/interface/expr.h"
#include "sema/interface/const.h"
#include "llvm/interface/llvm.h"
#include <llvm-c/Types.h>

LLVMValueRef llvm_binop(LlvmBackend *llvm, LLVMValueRef left, LLVMValueRef right, AstBinopKind kind);
LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr);
LLVMValueRef llvm_expr_get(LlvmBackend *llvm, AstExpr *expr);
LLVMValueRef llvm_const(LlvmBackend *llvm, SemaConst *constant);
