#pragma once

#include "llvm.h"

void llvm_module(LlvmBackend *llvm, AstModule *module);
void llvm_body_break(LlvmBackend *llvm, AstBody *body);
bool llvm_body(LlvmBackend *llvm, AstBody *body);
LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr, bool load);
LLVMValueRef llvm_call(LlvmBackend *llvm, AstCall *call);

LLVMValueRef llvm_resolve_path(LlvmBackend *llvm, AstPath *path);
LLVMValueRef llvm_resolve_inner_path(LlvmBackend *llvm, LLVMValueRef value, AstInnerPath *path);
