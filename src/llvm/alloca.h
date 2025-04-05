#pragma once

#include "llvm/interface/llvm.h"
#include <llvm-c/Types.h>

LLVMValueRef llvm_alloca(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef initial);
