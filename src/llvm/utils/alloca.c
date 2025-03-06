#include "llvm/utils/alloca.h"
#include "llvm/api.h"
#include "llvm/private.h"

LLVMValueRef llvm_alloca(LlvmBackend *llvm, LLVMTypeRef type) {
    LLVMPositionBuilderAtEnd(llvm_builder(llvm), llvm_definitions_block(llvm));
    LLVMValueRef result = LLVMBuildAlloca(llvm_builder(llvm), type, "");
    LLVMPositionBuilderAtEnd(llvm_builder(llvm), llvm_code_block(llvm));
    return result;
}
