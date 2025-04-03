#pragma once

#include "ast/node/decl/val.h"
#include "llvm/interface/llvm.h"

void llvm_setup_val_info(LlvmBackend *llvm, AstValInfo *info, const char *name);
void llvm_setup_val_decl(LlvmBackend *llvm, AstValDecl *val_decl, const char *name);
void llvm_emit_val_decl(LlvmBackend *llvm, AstValDecl *decl);

