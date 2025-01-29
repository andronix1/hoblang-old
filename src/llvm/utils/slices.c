#include "slices.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of) {
    LLVMTypeRef fields[2] = { LLVMInt64Type(), of };
    return LLVMStructType(fields, 2, false);
}

LLVMValueRef llvm_slice_len(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice) {
    LLVMValueRef indices[2] = {
        LLVMConstInt(LLVMInt32Type(), 0, false),
        LLVMConstInt(LLVMInt32Type(), 0, false)
    };
    return LLVMBuildGEP2(llvm->builder, type, slice, indices, 2, "slice_len");
}

LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice) {
    LLVMValueRef indices[2] = {
        LLVMConstInt(LLVMInt32Type(), 0, false),
        LLVMConstInt(LLVMInt32Type(), 1, false)
    };
    return LLVMBuildGEP2(llvm->builder, type, slice, indices, 2, "slice_ptr");
}

LLVMValueRef llvm_slice_from_array(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array, size_t len) {
    LLVMTypeRef slice_type = llvm_slice_type(of);
    LLVMValueRef indices[1] = {
        LLVMConstInt(LLVMInt32Type(), 0, false)
    };
    LLVMValueRef arr_alloca = LLVMBuildAlloca(llvm->builder, LLVMArrayType(of, len), "arr_alloca");
    LLVMBuildStore(llvm->builder, array, arr_alloca);
    LLVMValueRef array_ptr = LLVMBuildBitCast(llvm->builder, arr_alloca, LLVMPointerType(LLVMPointerType(of, 0), 0), "");
    // LLVMValueRef array_ptr = LLVMBuildGEP2(llvm->builder, LLVMPointerType(of, 0), array, indices, 1, "array_ptr");

    LLVMValueRef slice = LLVMBuildAlloca(llvm->builder, slice_type, "slice");
    LLVMBuildStore(llvm->builder, LLVMConstInt(LLVMInt64Type(), len, false), llvm_slice_len(llvm, slice_type, slice));
    LLVMBuildStore(llvm->builder, array_ptr, llvm_slice_ptr(llvm, slice_type, slice));
    return slice;
}