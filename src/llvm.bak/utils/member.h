#pragma once

#include <stdbool.h>
#include <llvm-c/Core.h>
#include "llvm/llvm.h"

LLVMValueRef llvm_get_member(LlvmBackend *llvm, LLVMTypeRef type, LLVMTypeRef member_type, LLVMValueRef value, size_t index, bool load);
