#pragma once

#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include "core/log.h"
#include "core/assert.h"
#include "parser/ast.h"
#include "sema/type.h"

typedef struct {
	LLVMValueRef value;
	LLVMTypeRef type;
	Slice *name;
} LlvmValue;

typedef LlvmValue* LlvmScope;

typedef struct {
	LLVMTargetMachineRef machine;
	LLVMModuleRef module;
	LLVMValueRef func;
	LLVMBuilderRef builder;
	LlvmScope *scopes;
} LlvmBackend;

bool llvm_init(LlvmBackend *llvm);
LlvmValue *llvm_resolve(LlvmBackend *llvm, Slice *name);
LLVMTypeRef llvm_resolve_type_of(LlvmBackend *llvm, Slice *name);
LLVMValueRef llvm_resolve_value(LlvmBackend *llvm, Slice *name);
LLVMTypeRef llvm_resolve_type(SemaType *type);
bool llvm_write_module(LlvmBackend *llvm, char *output_path);
void llvm_push_value(LlvmBackend *llvm, LlvmValue *value);
void llvm_pop_scope(LlvmBackend *llvm);
void llvm_push_scope(LlvmBackend *llvm);
