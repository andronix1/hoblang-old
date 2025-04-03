#include "type.h"
#include <llvm-c/Core.h>
#include "core/vec.h"
#include "sema/type/type.h"
#include "ast/node/decl/struct.h"
#include "ast/shared/type.h"
#include "core/assert.h"
#include "llvm/slice.h"

LLVMTypeRef llvm_optional_type(LLVMTypeRef of) {
    LLVMTypeRef types[] = {
        LLVMInt1Type(),
        of
    };
    return LLVMStructType(types, sizeof(types) / sizeof(types[0]), true);
}

LLVMTypeRef llvm_type(SemaType* type) {
    switch (type->kind) {
        case SEMA_TYPE_PRIMITIVE: {
            switch (type->primitive.kind) {
                case SEMA_PRIMITIVE_INT:
                    switch (type->primitive.integer) {
                        case SEMA_PRIMITIVE_UINT8: case SEMA_PRIMITIVE_INT8: return LLVMInt8Type();
                        case SEMA_PRIMITIVE_INT16: case SEMA_PRIMITIVE_UINT16: return LLVMInt16Type();
                        case SEMA_PRIMITIVE_INT32: case SEMA_PRIMITIVE_UINT32: return LLVMInt32Type();
                        case SEMA_PRIMITIVE_INT64: case SEMA_PRIMITIVE_UINT64: return LLVMInt64Type();
                    }
                    assert(0, "invalid integer");
                case SEMA_PRIMITIVE_FLOAT:
                    switch (type->primitive.float_type) {
                        case SEMA_PRIMITIVE_FLOAT32: return LLVMFloatType();
                        case SEMA_PRIMITIVE_FLOAT64: return LLVMDoubleType();
                    }
                    assert(0, "invalid float");
                case SEMA_PRIMITIVE_BOOL: return LLVMInt1Type();
                case SEMA_PRIMITIVE_VOID: return LLVMVoidType();
            }
            assert(0, "invalid primitive");
        }
        case SEMA_TYPE_OPTIONAL: return llvm_optional_type(llvm_type(type->optional_of));
        case SEMA_TYPE_POINTER: return LLVMPointerType(llvm_type(type->pointer_to), 0);
        case SEMA_TYPE_FUNCTION: {
            size_t args_count = vec_len(type->func.args); 
            LLVMTypeRef *types = alloca(sizeof(LLVMTypeRef) * args_count);
            for (size_t i = 0; i < args_count; i++) {
                types[i] = llvm_type(type->func.args[i]);
            }
            return LLVMFunctionType(llvm_type(type->func.returns), types, args_count, false); 
        }
        case SEMA_TYPE_STRUCT: {
            size_t fields_count = vec_len(type->struct_decl->fields);
            LLVMTypeRef *types = alloca(sizeof(LLVMTypeRef) * fields_count);
            for (size_t i = 0; i < fields_count; i++) {
                types[i] = llvm_type(type->struct_decl->fields[i].type->sema.type);
            }
            return LLVMStructType(types, fields_count, false);
        }
        case SEMA_TYPE_SLICE: return llvm_slice_type(llvm_type(type->slice_of));
        case SEMA_TYPE_ARRAY: return LLVMArrayType(llvm_type(type->array.of), type->array.length);
        case SEMA_TYPE_GENERIC: assert(0, "generics are NIY");
    }
    assert(0, "invalid type");
}
