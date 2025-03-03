#pragma once

#include <llvm-c/Core.h>
#include <stdbool.h>
#include "sema/value.h"
#include "llvm/llvm.h"
#include "ast/api/path.h"

LLVMValueRef llvm_resolve_path(LlvmBackend *llvm, LLVMValueRef value, AstPath *path, SemaValue *from);
