#pragma once

#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include "core/log.h"
#include "parser/ast.h"

typedef struct {
	LLVMValueRef value;
	LLVMTypeRef type;
	FatPtr *name;
} LlvmValue;

typedef Vec LlvmScope;
typedef Vec LlvmScopeStack;

typedef struct {
	LLVMTargetMachineRef machine;
	LLVMModuleRef module;
	LLVMValueRef func;
	LLVMBuilderRef builder;
	LlvmScopeStack scopes;
} LlvmBackend;

bool llvm_init(LlvmBackend *llvm);
void llvm_module(LlvmBackend *llvm, AstModule *module, char *output_path);
