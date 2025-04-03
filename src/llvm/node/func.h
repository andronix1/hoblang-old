#pragma once

#include "ast/node/decl/func.h"
#include "llvm/interface/llvm.h"

void llvm_setup_func_info(LlvmBackend *llvm, AstFuncInfo *info, const char *name);
void llvm_setup_func_decl(LlvmBackend *llvm, AstFuncDecl *func_decl, const char *name);
void llvm_emit_func_decl(LlvmBackend *llvm, AstFuncDecl *decl);

