#pragma once

#include "ast/api/val_decl.h"
#include "llvm/llvm.h"

void llvm_val_decl_external(LlvmBackend *llvm, AstValDecl *val_decl);
void llvm_val_decl_global_init(LlvmBackend *llvm, AstValDecl *val_decl);
void llvm_val_decl_global_set(LlvmBackend *llvm, AstValDecl *val_decl);
void llvm_val_decl(LlvmBackend *llvm, AstValDecl *val_decl);
