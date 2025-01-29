#include "../parts.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of);
LLVMValueRef llvm_slice_len(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice);
LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice);
LLVMValueRef llvm_slice_from_array(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array, size_t len);