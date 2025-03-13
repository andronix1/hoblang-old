#include <llvm-c/Core.h>
#include <alloca.h>
#include "sema/type/private.h"
#include "ast/private/type.h"
#include "ast/private/module_node.h"
#include "llvm-c/Types.h"
#include "llvm/llvm.h"
#include "llvm/parts/type.h"
#include "llvm/parts/types/slice.h"
#include "llvm/parts/types/optional.h"
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
			switch (type->primitive.type) {
				case SEMA_PRIMITIVE_BOOL: return LLVMInt1Type();
				case SEMA_PRIMITIVE_VOID: return LLVMVoidType();
				case SEMA_PRIMITIVE_FLOAT:
					switch (type->primitive.float_type) {
						case SEMA_PRIMITIVE_FLOAT32: return LLVMFloatType();
						case SEMA_PRIMITIVE_FLOAT64: return LLVMDoubleType();
					}
					break;
				case SEMA_PRIMITIVE_INT:
					switch (type->primitive.integer) {
						case SEMA_PRIMITIVE_INT8: case SEMA_PRIMITIVE_UINT8: return LLVMInt8Type();
						case SEMA_PRIMITIVE_INT16: case SEMA_PRIMITIVE_UINT16: return LLVMInt16Type();
						case SEMA_PRIMITIVE_INT32: case SEMA_PRIMITIVE_UINT32: return LLVMInt32Type();
						case SEMA_PRIMITIVE_INT64: case SEMA_PRIMITIVE_UINT64: return LLVMInt64Type();
					}
					break;
			}
			assert(0, "invalid primitive {int}", type->primitive);
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
		case SEMA_TYPE_OPTIONAL: return llvm_opt_type(llvm_resolve_type(type->optional_of));
		case SEMA_TYPE_SLICE: return llvm_slice_type(llvm_resolve_type(type->slice_of));
		case SEMA_TYPE_ARRAY: {
			return LLVMArrayType(llvm_resolve_type(type->array.of), type->array.length);
		}
		case SEMA_TYPE_POINTER: {
			return LLVMPointerType(llvm_resolve_type(type->ptr_to), 0);
		}
        case SEMA_TYPE_GENERIC:
	        assert(0, "trying to resolve generic", type->type);
	}
	assert(0, "invalid type {int}", type->type);
}

LLVMValueRef llvm_type_sizeof(LlvmBackend *llvm, LLVMTypeRef type, LLVMTypeRef output_type) {
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
        output_type,
        "sizeof.int"
    );
}
