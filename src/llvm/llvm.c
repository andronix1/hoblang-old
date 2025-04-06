#include "llvm.h"
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>

LlvmState llvm_switch_state(LlvmBackend *llvm, LlvmState state) {
    LlvmState old_state = llvm->state;
    llvm->state = state;
    LLVMPositionBuilderAtEnd(llvm->builder, state.code);
    return old_state;
}
