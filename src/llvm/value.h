#pragma once

#include "sema/interface/value.h"
#include "llvm/interface/llvm.h"
#include <llvm-c/Types.h>

LLVMValueRef llvm_value_get(LlvmBackend *llvm, LLVMValueRef source, SemaValue *value);
