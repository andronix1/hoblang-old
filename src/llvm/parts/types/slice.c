#include "llvm/private.h"
#include "llvm/parts/types/slice.h"

LLVMTypeRef llvm_slice_type(LLVMTypeRef of) {
    LLVMTypeRef fields[2] = { LLVMInt64Type(), LLVMPointerType(of, 0) };
    return LLVMStructType(fields, 2, false);
}

LLVMValueRef llvm_slice_len(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice) {
    LLVMValueRef indices[2] = {
        LLVMConstInt(LLVMInt32Type(), 0, false),
        LLVMConstInt(LLVMInt32Type(), 0, false)
    };
    return LLVMBuildGEP2(llvm_builder(llvm), type, slice, indices, 2, "slice_len");
}

LLVMValueRef llvm_slice_ptr(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef slice) {
    LLVMValueRef indices[2] = {
        LLVMConstInt(LLVMInt32Type(), 0, false),
        LLVMConstInt(LLVMInt32Type(), 1, false)
    };
    return LLVMBuildGEP2(llvm_builder(llvm), type, slice, indices, 2, "slice_ptr");
}

LLVMValueRef llvm_alloca_slice(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef ptr, size_t len) {
    LLVMTypeRef slice_type = llvm_slice_type(of);
    LLVMValueRef slice = LLVMBuildAlloca(llvm_builder(llvm), slice_type, "slice");
    LLVMBuildStore(llvm_builder(llvm), LLVMConstInt(LLVMInt64Type(), len, false), llvm_slice_len(llvm, slice_type, slice));
    LLVMBuildStore(llvm_builder(llvm), ptr, llvm_slice_ptr(llvm, slice_type, slice));
    return LLVMBuildLoad2(llvm_builder(llvm), slice_type, slice, "loaded_slice");
}

LLVMValueRef llvm_slice_from_array_ptr(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef array, size_t len) {
    // LLVMValueRef arr_alloca = LLVMBuildAlloca(llvm_builder(llvm), LLVMArrayType(of, len), "arr_alloca");
    // LLVMBuildStore(llvm_builder(llvm), array, arr_alloca);
    // LLVMValueRef array_ptr = LLVMBuildBitCast(llvm_builder(llvm), arr_alloca, LLVMPointerType(of, 0), "");
	LLVMValueRef indices[] = {
		LLVMConstInt(LLVMInt32Type(), 0, false),
		LLVMConstInt(LLVMInt32Type(), 0, false)
	};
	LLVMValueRef array_ptr = LLVMBuildGEP2(
		llvm_builder(llvm),
		of,
		array,
		indices, 2,
		"array_ptr"
	);

    return llvm_alloca_slice(llvm, of, array_ptr, len);
}

LLVMValueRef llvm_slice_from_str(LlvmBackend *llvm, Slice *str) {
    LLVMValueRef str_global = LLVMAddGlobal(llvm_current_module(llvm), LLVMArrayType(LLVMInt8Type(), str->len), "str");
    LLVMSetInitializer(str_global, LLVMConstString(str->str, str->len, true));
    LLVMValueRef str_ptr = LLVMBuildBitCast(llvm_builder(llvm), str_global, LLVMPointerType(LLVMInt8Type(), 0), "");

    return llvm_alloca_slice(llvm, LLVMInt8Type(), str_ptr, str->len);
}
