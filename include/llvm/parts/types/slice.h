#include <llvm-c/Core.h>
#include "llvm/llvm.h"
#include <stdbool.h>
#include "core/slice.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of);
LLVMValueRef llvm_slice_len(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice, bool load);
LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef slice, bool load);
LLVMValueRef llvm_slice_from_array_ptr(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array_ptr, size_t len);
LLVMValueRef llvm_slice_from_str(LlvmBackend *llvm, Slice *str);
