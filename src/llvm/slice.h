#pragma once

#include "core/slice/slice.h"
#include "sema/interface/value.h"
#include "llvm/interface/llvm.h"
#include <llvm-c/Types.h>

LLVMTypeRef llvm_slice_type(LLVMTypeRef of);
LLVMValueRef llvm_slice_new(LlvmBackend *llvm, SemaType *type, LLVMValueRef ptr, size_t length);
LLVMValueRef llvm_slice_from_string(LlvmBackend *llvm, Slice *string); 
LLVMValueRef llvm_slice_length(LlvmBackend *llvm, SemaValue *value, LLVMValueRef slice);
LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, SemaValue *value, LLVMValueRef slice);
