#pragma once

#include <llvm-c/Core.h>
#include "llvm/llvm.h"
#include "sema/type/private.h"

LLVMTypeRef llvm_sema_function_type(SemaFunction *func);
LLVMTypeRef llvm_resolve_type(SemaType *type);
LLVMValueRef llvm_type_sizeof(LlvmBackend *llvm, LLVMTypeRef type);
