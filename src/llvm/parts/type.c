#include <llvm-c/Core.h>
#include <alloca.h>
#include "sema/type/private.h"
#include "ast/private/type.h"
#include "ast/private/module_node.h"
#include "llvm-c/Types.h"
#include "llvm/llvm.h"
#include "llvm/parts/type.h"
#include "llvm/private.h"
#include "core/vec.h"

LLVMTypeRef llvm_sema_function_type(SemaFunction *func) {
	LLVMTypeRef *params = alloca(sizeof(LLVMTypeRef) * vec_len(func->args));
	for (size_t i = 0; i < vec_len(func->args); i++) {
		params[i] = llvm_resolve_type(func->args[i]);
	}
	return LLVMFunctionType(llvm_resolve_type(func->returning), params, vec_len(func->args), false);
}

LLVMTypeRef llvm_resolve_type(SemaType *type) {
	assert(type, "type is unresolved");
	switch (type->type) {
		case SEMA_TYPE_STRUCT: {
			LLVMTypeRef *elements = alloca(sizeof(LLVMTypeRef) * vec_len(type->struct_def->members));
			for (size_t i = 0; i < vec_len(type->struct_def->members); i++) {
				elements[i] = llvm_resolve_type(type->struct_def->members[i].type->sema);
			}
			return LLVMStructType(elements, vec_len(type->struct_def->members), false);
		}
		case SEMA_TYPE_PRIMITIVE:
			switch (type->primitive) {
				case PRIMITIVE_I8: case PRIMITIVE_U8: return LLVMInt8Type();
				case PRIMITIVE_I16: case PRIMITIVE_U16: return LLVMInt16Type();
				case PRIMITIVE_I32: case PRIMITIVE_U32: return LLVMInt32Type();
				case PRIMITIVE_I64: case PRIMITIVE_U64: return LLVMInt64Type();
				case PRIMITIVE_BOOL: return LLVMInt1Type();
				case PRIMITIVE_VOID: return LLVMVoidType();
			}
			assert(0, "invalid primitive {int}", type->primitive);
			return NULL;
		case SEMA_TYPE_FUNCTION: {
			LLVMTypeRef *params = alloca(sizeof(LLVMTypeRef) * vec_len(type->func.args));
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				params[i] = llvm_resolve_type(type->func.args[i]);
			}
			return LLVMPointerType(
				LLVMFunctionType(llvm_resolve_type(type->func.returning), params, vec_len(type->func.args), false /* IsVarArg */),
				0
			);
		}
		case SEMA_TYPE_SLICE: {
			LLVMTypeRef elements[2] = { LLVMInt64Type(), LLVMPointerType(llvm_resolve_type(type->slice_of), 0) };
			return LLVMStructType(elements, 2, false);
		}
		case SEMA_TYPE_ARRAY: {
			return LLVMArrayType(llvm_resolve_type(type->array.of), type->array.length);
		}
		case SEMA_TYPE_POINTER: {
			return LLVMPointerType(llvm_resolve_type(type->ptr_to), 0);
		}
	}
	assert(0, "invalid type {int}", type->type);
}

LLVMValueRef llvm_type_sizeof(LlvmBackend *llvm, LLVMTypeRef type) {
    LLVMValueRef indices[] = {
        LLVMConstInt(LLVMInt32Type(), 1, false)
    };
    return LLVMBuildPtrToInt(
        llvm_builder(llvm),
        LLVMBuildGEP2(
            llvm_builder(llvm),
            type,
            LLVMConstPointerNull(LLVMPointerType(type, 0)),
            indices, 1,
            "sizeof.gep"
        ),
        LLVMInt32Type(),
        "sizeof.int"
    );
}
