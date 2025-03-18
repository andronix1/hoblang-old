#pragma once 

#include "ast/private/module_node.h"
#include "llvm/llvm.h"
#include <llvm-c/Types.h>

void llvm_emit_func_decl(LlvmBackend *llvm, LLVMValueRef func, AstFuncDecl *func_decl);
