#pragma once

#include <llvm-c/Core.h>
#include <stdbool.h>
#include "llvm/llvm.h"
#include "ast/api/path.h"

LLVMValueRef llvm_resolve_path(LlvmBackend *llvm, AstPath *path);
LLVMValueRef llvm_resolve_inner_path(LlvmBackend *llvm, LLVMValueRef value, AstInnerPath *path);
