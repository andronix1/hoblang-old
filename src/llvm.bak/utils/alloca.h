#pragma once

#include "llvm/llvm.h"
#include <llvm-c/Core.h>

LLVMValueRef llvm_alloca(LlvmBackend *llvm, LLVMTypeRef type);
