#include "slices.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of) {
    LLVMTypeRef fields[2] = { LLVMInt64Type(), LLVMPointerType(of, 0) };
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

LLVMValueRef llvm_alloca_slice(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef ptr, size_t len) {
    LLVMTypeRef slice_type = llvm_slice_type(of);
    LLVMValueRef slice = LLVMBuildAlloca(llvm->builder, slice_type, "slice");
    LLVMBuildStore(llvm->builder, LLVMConstInt(LLVMInt64Type(), len, false), llvm_slice_len(llvm, slice_type, slice));
    LLVMBuildStore(llvm->builder, ptr, llvm_slice_ptr(llvm, slice_type, slice));
    return LLVMBuildLoad2(llvm->builder, slice_type, slice, "loaded_slice");
}

LLVMValueRef llvm_slice_from_array(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array, size_t len) {
    LLVMTypeRef slice_type = llvm_slice_type(of);
    // LLVMValueRef arr_alloca = LLVMBuildAlloca(llvm->builder, LLVMArrayType(of, len), "arr_alloca");
    // LLVMBuildStore(llvm->builder, array, arr_alloca);
    LLVMValueRef array_ptr = LLVMBuildBitCast(llvm->builder, array/* arr_alloca */, LLVMPointerType(of, 0), "");

    return llvm_alloca_slice(llvm, of, array_ptr, len);
}

LLVMValueRef llvm_slice_from_str(LlvmBackend *llvm, Slice *str) {
    LLVMTypeRef slice_type = llvm_slice_type(LLVMInt8Type());
    LLVMValueRef str_global = LLVMAddGlobal(llvm->module, LLVMArrayType(LLVMInt8Type(), str->len), "str");
    LLVMSetInitializer(str_global, LLVMConstString(str->str, str->len, true));
    LLVMValueRef str_ptr = LLVMBuildBitCast(llvm->builder, str_global, LLVMPointerType(LLVMInt8Type(), 0), "");

    return llvm_alloca_slice(llvm, LLVMInt8Type(), str_ptr, str->len);
}
