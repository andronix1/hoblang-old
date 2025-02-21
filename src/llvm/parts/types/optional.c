#include "llvm/private.h"
#include "llvm/parts/types/optional.h"

LLVMTypeRef llvm_opt_type(LLVMTypeRef of) {
    LLVMTypeRef fields[2] = { LLVMInt1Type(), of };
    return LLVMStructType(fields, 2, false);
}

LLVMValueRef llvm_opt_is_null(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef opt) {
    LLVMValueRef indices[2] = {
        LLVMConstInt(LLVMInt32Type(), 0, false),
        LLVMConstInt(LLVMInt32Type(), 0, false)
    };
    return LLVMBuildGEP2(llvm_builder(llvm), type, opt, indices, 2, "opt.is_null");
}

LLVMValueRef llvm_opt_value(LlvmBackend *llvm, LLVMTypeRef type, LLVMValueRef opt, bool load) {
    if (LLVMGetTypeKind(LLVMTypeOf(opt)) == LLVMPointerTypeKind) {
        LLVMValueRef indices[2] = {
            LLVMConstInt(LLVMInt32Type(), 0, false),
            LLVMConstInt(LLVMInt32Type(), 1, false)
        };
        LLVMValueRef value = LLVMBuildGEP2(llvm_builder(llvm), type, opt, indices, 2, "opt.value");
        if (load) {
            return LLVMBuildLoad2(
                llvm_builder(llvm),
                type,
                value, 
                "opt.unwrap.load"
            );
        }
        return value;
    } else { 
        return LLVMBuildExtractValue(llvm_builder(llvm), opt, 1, "opt.value");
    }
}

LLVMValueRef llvm_opt_null(LlvmBackend *llvm, LLVMTypeRef of) {
    LLVMTypeRef opt_type = llvm_opt_type(of);
    LLVMValueRef opt = LLVMBuildAlloca(llvm_builder(llvm), opt_type, "opt");
    LLVMBuildStore(llvm_builder(llvm), LLVMConstInt(LLVMInt1Type(), 1, false), llvm_opt_is_null(llvm, opt_type, opt));
    return LLVMBuildLoad2(llvm_builder(llvm), opt_type, opt, "loaded_opt");
}

LLVMValueRef llvm_opt_wrap(LlvmBackend *llvm, LLVMTypeRef of, LLVMValueRef value) {
    LLVMTypeRef opt_type = llvm_opt_type(of);
    LLVMValueRef opt = LLVMBuildAlloca(llvm_builder(llvm), opt_type, "opt");
    LLVMBuildStore(llvm_builder(llvm), LLVMConstInt(LLVMInt1Type(), 0, false), llvm_opt_is_null(llvm, opt_type, opt));
    LLVMBuildStore(llvm_builder(llvm), value, llvm_opt_value(llvm, opt_type, opt, false));
    return LLVMBuildLoad2(llvm_builder(llvm), opt_type, opt, "loaded_opt");
}

