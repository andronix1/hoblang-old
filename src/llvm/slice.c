#include "slice.h" 
#include "sema/interface/value.h"
#include <llvm-c/Core.h>
#include "llvm/alloca.h"
#include "llvm/llvm.h"
#include "llvm/type.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of) {
    LLVMTypeRef types[] = {
        LLVMInt64Type(), // TODO: arch
        LLVMPointerType(of, 0)
    };
    return LLVMStructType(types, sizeof(types) / sizeof(types[0]), true);
}

LLVMValueRef llvm_slice_length_ptr(LlvmBackend *llvm, SemaType *type, LLVMValueRef slice) {
    LLVMValueRef indices[1] = { LLVMConstInt(LLVMInt32Type(), 0, false) };
    return LLVMBuildGEP2(llvm->builder, llvm_type(type), slice, indices, 1, ""); 
}

LLVMValueRef llvm_slice_ptr_ptr(LlvmBackend *llvm, SemaType *type, LLVMValueRef slice) {
    LLVMValueRef indices[1] = { LLVMConstInt(LLVMInt32Type(), 1, false) };
    return LLVMBuildGEP2(llvm->builder, llvm_type(type), slice, indices, 1, ""); 
}

LLVMValueRef llvm_slice_new_const(LLVMValueRef ptr, size_t length) {
    LLVMValueRef vals[2] = {
        LLVMConstInt(LLVMInt64Type(), length, false), // TODO: arch
        ptr
    };
    return LLVMConstStruct(vals, 2, true);
}

LLVMValueRef llvm_slice_new(LlvmBackend *llvm, SemaType *type, LLVMValueRef ptr, size_t length) {
    LLVMTypeRef ltype = llvm_type(type); 
    LLVMValueRef res = llvm_alloca(llvm, ltype, NULL);
    // TODO: arch-dependent
    LLVMBuildStore(
        llvm->builder,
        LLVMConstInt(LLVMInt64Type(), length, false),
        llvm_slice_length_ptr(llvm, type, res)
    );
    LLVMBuildStore(
        llvm->builder,
        ptr,
        llvm_slice_ptr_ptr(llvm, type, res)
    );
    return LLVMBuildLoad2(llvm->builder, ltype, res, "");
}

LLVMValueRef llvm_slice_from_string(LlvmBackend *llvm, Slice *string) {
    LLVMValueRef str_global = LLVMAddGlobal(llvm->module, LLVMArrayType2(LLVMInt8Type(), string->len), "");
    LLVMSetInitializer(str_global, LLVMConstString(string->str, string->len, true));
    LLVMValueRef str_ptr = LLVMBuildBitCast(llvm->builder, str_global, LLVMPointerType(LLVMInt8Type(), 0), "");

    return llvm_slice_new_const(str_ptr, string->len);
}

LLVMValueRef llvm_slice_length(LlvmBackend *llvm, SemaValue *value, LLVMValueRef slice) {
    SemaType *type = sema_value_is_runtime(value);
    if (sema_value_is_var(value)) {
        LLVMValueRef indices[] = {
            LLVMConstInt(LLVMInt32Type(), 0, false),
            LLVMConstInt(LLVMInt32Type(), 0, false)
        };
        return LLVMBuildGEP2(llvm->builder, llvm_type(type), slice, indices, 2, ""); 
    } else {
        return LLVMBuildExtractValue(llvm->builder, slice, 0, "");
    }
}

LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, SemaValue *value, LLVMValueRef slice) {
    SemaType *type = sema_value_is_runtime(value);
    if (sema_value_is_var(value)) {
        LLVMValueRef indices[] = {
            LLVMConstInt(LLVMInt32Type(), 0, false),
            LLVMConstInt(LLVMInt32Type(), 1, false)
        };
        return LLVMBuildGEP2(llvm->builder, llvm_type(type), slice, indices, 2, ""); 
    } else {
        return LLVMBuildExtractValue(llvm->builder, slice, 1, "");
    }
}
