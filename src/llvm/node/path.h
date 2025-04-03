#pragma once

#include "ast/interface/path.h"
#include "llvm/interface/llvm.h"
#include <llvm-c/Types.h>

LLVMValueRef llvm_path(LlvmBackend *llvm, AstPath *path);
