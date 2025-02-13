#pragma once

#include <llvm-c/Core.h>
#include "llvm/llvm.h"

LLVMBuilderRef llvm_builder(LlvmBackend *llvm);
LLVMModuleRef llvm_current_module(LlvmBackend *llvm);
LLVMValueRef llvm_current_func(LlvmBackend *llvm);
void llvm_set_current_func(LlvmBackend *llvm, LLVMValueRef func);
