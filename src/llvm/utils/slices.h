#include "../parts.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of);
LLVMValueRef llvm_slice_len(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice);
LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice);
LLVMValueRef llvm_slice_ptr_direct(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice);
LLVMValueRef llvm_slice_from_array_ptr(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array_ptr, size_t len);
LLVMValueRef llvm_slice_from_str(LlvmBackend *llvm, Slice *str);
