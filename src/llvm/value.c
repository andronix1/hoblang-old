#include "value.h"
#include "llvm/llvm.h"
#include "llvm/type.h"
#include <llvm-c/Core.h>

LLVMValueRef llvm_value_get(LlvmBackend *llvm, LLVMValueRef source, SemaValue *value) {
    SemaType *type = sema_value_is_var(value);
    if (type) {
        return LLVMBuildLoad2(
            llvm->builder,
            llvm_type(type),
            source,
            ""
        );
    }
    return source;
}
