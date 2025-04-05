#include "alloca.h" 
#include <llvm-c/Core.h>
#include "llvm/llvm.h"

LLVMValueRef llvm_alloca(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef initial) {
    LLVMValueRef alloca = LLVMBuildAlloca(llvm->builder, type, "");
    if (initial) {
        LLVMBuildStore(llvm->builder, initial, alloca);
    }
    return alloca;
}
