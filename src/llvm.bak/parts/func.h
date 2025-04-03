#pragma once

#include "ast/private/body.h"
#include "ast/private/func_info.h"
#include "sema/module/decls/decls.h"
#include "llvm/llvm.h"
#include <llvm-c/Types.h>

void llvm_emit_func(LlvmBackend *llvm, LLVMValueRef func, SemaDecl *ext, AstFuncTypeInfo *info, AstBody *body);
