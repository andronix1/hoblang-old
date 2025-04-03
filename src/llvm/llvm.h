#pragma once

#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>

typedef struct LlvmBackend {
	LLVMTargetMachineRef machine;
	LLVMModuleRef module;
	LLVMValueRef func;
	LLVMBuilderRef builder;
} LlvmBackend;

LLVMValueRef llvm_switch_func(LlvmBackend *llvm, LLVMValueRef func);
