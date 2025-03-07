#include <llvm-c/Core.h>
#include "llvm/llvm.h"
#include <stdbool.h>

LLVMTypeRef llvm_opt_type(LLVMTypeRef of);
LLVMValueRef llvm_opt_is_null(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef opt, bool load);
LLVMValueRef llvm_opt_value(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef opt, bool load);
LLVMValueRef llvm_opt_wrap(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef value);
LLVMValueRef llvm_opt_wrap_const(LLVMValueRef value);
LLVMValueRef llvm_opt_null(LLVMTypeRef of);
