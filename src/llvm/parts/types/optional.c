#include "llvm/private.h"
#include "llvm/parts/types/optional.h"
#include "llvm/utils/alloca.h"
#include "llvm/utils/member.h"
#include <llvm-c/Core.h>

LLVMTypeRef llvm_opt_type(LLVMTypeRef of) {
    LLVMTypeRef fields[2] = { LLVMInt1Type(), of };
    return LLVMStructType(fields, 2, false);
}

LLVMValueRef llvm_opt_is_null(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef opt, bool load) {
    return llvm_get_member(llvm, type, LLVMInt1Type(), opt, 0, load);
}

LLVMValueRef llvm_opt_value(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef opt, bool load) {
    return llvm_get_member(llvm, llvm_opt_type(of), of, opt, 1, load);
}

LLVMValueRef llvm_opt_null(LLVMTypeRef of) {
    LLVMValueRef values[] = {
        LLVMConstInt(LLVMInt1Type(), 1, false),
        LLVMGetUndef(of)
    };
    return LLVMConstStruct(values, 2, false);
}

LLVMValueRef llvm_opt_wrap_const(LLVMValueRef value) {
    LLVMValueRef values[] = {
        LLVMConstInt(LLVMInt1Type(), 1, false),
        value
    };
    return LLVMConstStruct(values, 2, false);
}

LLVMValueRef llvm_opt_wrap(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef value) {
    LLVMTypeRef opt_type = llvm_opt_type(of);
    LLVMValueRef opt = llvm_alloca(llvm, opt_type);
    LLVMBuildStore(llvm_builder(llvm), LLVMConstInt(LLVMInt1Type(), 0, false), llvm_opt_is_null(llvm, opt_type, opt, false));
    LLVMBuildStore(llvm_builder(llvm), value, llvm_opt_value(llvm, opt_type, opt, false));
    return LLVMBuildLoad2(llvm_builder(llvm), opt_type, opt, "loaded_opt");
}

