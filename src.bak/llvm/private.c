#include "llvm/private.h"
#include "llvm/impl.h"
#include "core/vec.h"
#include "ast/private/type.h"
#include "sema/type/private.h"

LLVMModuleRef llvm_current_module(LlvmBackend *llvm) {
    return llvm->module;
}

LLVMValueRef llvm_current_func(LlvmBackend *llvm) {
    return llvm->func;
}

void llvm_set_current_func(LlvmBackend *llvm, LLVMValueRef func) {
    llvm->func = func;
}

LLVMBuilderRef llvm_builder(LlvmBackend *llvm) {
    return llvm->builder;
}

LLVMBasicBlockRef llvm_code_block(LlvmBackend *llvm) {
    return llvm->code_block;
}

LLVMBasicBlockRef llvm_definitions_block(LlvmBackend *llvm) {
    return llvm->definitions_block;
}

void llvm_set_code_block(LlvmBackend *llvm, LLVMBasicBlockRef block) {
    llvm->code_block = block;
}

void llvm_set_definitions_block(LlvmBackend *llvm, LLVMBasicBlockRef block) {
    llvm->definitions_block = block;
}
