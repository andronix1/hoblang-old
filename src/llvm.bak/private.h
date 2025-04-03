#pragma once

#include <llvm-c/Core.h>
#include "llvm-c/Types.h"
#include "llvm/llvm.h"

LLVMBuilderRef llvm_builder(LlvmBackend *llvm);
LLVMModuleRef llvm_current_module(LlvmBackend *llvm);
LLVMValueRef llvm_current_func(LlvmBackend *llvm);
void llvm_set_current_func(LlvmBackend *llvm, LLVMValueRef func);

LLVMBasicBlockRef llvm_code_block(LlvmBackend *llvm);
LLVMBasicBlockRef llvm_definitions_block(LlvmBackend *llvm);
void llvm_set_code_block(LlvmBackend *llvm, LLVMBasicBlockRef block);
void llvm_set_definitions_block(LlvmBackend *llvm, LLVMBasicBlockRef block);
