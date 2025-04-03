#include "llvm.h"
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>

LLVMValueRef llvm_switch_func(LlvmBackend *llvm, LLVMValueRef func) {
    LLVMValueRef temp = llvm->func;
    llvm->func = func;
    return temp;
}
