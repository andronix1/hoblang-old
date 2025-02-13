#pragma once

#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>

typedef struct LlvmBackend {
	LLVMTargetMachineRef machine;
	LLVMModuleRef module;
	LLVMValueRef func;
	LLVMBuilderRef builder;
} LlvmBackend;

