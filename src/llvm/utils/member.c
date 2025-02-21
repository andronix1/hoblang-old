#include <llvm-c/Core.h>
#include "llvm/utils/member.h"
#include "llvm/private.h"

LLVMValueRef llvm_get_member(LlvmBackend *llvm, LLVMTypeRef type, LLVMTypeRef member_type, LLVMValueRef value, size_t index, bool load) {
    if (LLVMGetTypeKind(LLVMTypeOf(value)) == LLVMPointerTypeKind) {
        LLVMValueRef indices[2] = {
            LLVMConstInt(LLVMInt32Type(), 0, false),
            LLVMConstInt(LLVMInt32Type(), index, false)
        };
        LLVMValueRef member_ptr = LLVMBuildGEP2(llvm_builder(llvm), type, value, indices, 2, "");
        if (load) {
            return LLVMBuildLoad2(
                llvm_builder(llvm),
                member_type,
                member_ptr,
                ""
            );
        }
        return member_ptr;
    } else { 
        return LLVMBuildExtractValue(llvm_builder(llvm), value, index, "");
    }
}
