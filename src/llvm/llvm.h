#pragma once

#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Analysis.h>
#include "core/log.h"
#include "core/assert.h"
#include "parser/api.h"
#include "sema/type/api.h"
#include "sema/module/api.h"
#include "sema/type/private.h"

typedef struct {
	LLVMTargetMachineRef machine;
	LLVMModuleRef module;
	LLVMValueRef func;
	LLVMBuilderRef builder;
} LlvmBackend;

bool llvm_init(LlvmBackend *llvm);
LLVMTypeRef llvm_sema_function_type(SemaFunction *func);
LLVMTypeRef llvm_resolve_type(SemaType *type);
void llvm_module_init(LlvmBackend *llvm, AstModule *module);
bool llvm_write_module(LlvmBackend *llvm, char *output_path);
bool llvm_write_module_ir(LlvmBackend *llvm, char *output_path);
