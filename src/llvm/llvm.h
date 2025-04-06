#pragma once

#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>

typedef struct {
    LLVMValueRef func;
    LLVMBasicBlockRef code, defs;
} LlvmState;

typedef struct LlvmBackend {
	LLVMTargetMachineRef machine;
	LLVMModuleRef module;
    LlvmState state;
	LLVMBuilderRef builder;
} LlvmBackend;

static inline LlvmState llvm_state(
    LLVMValueRef func,
    LLVMBasicBlockRef code,
    LLVMBasicBlockRef defs
) {
    LlvmState result = {
        .func = func,
        .code = code,
        .defs = defs
    };
    return result;
}

static inline void llvm_pos_defs(LlvmBackend *llvm, LLVMBasicBlockRef block) {
    llvm->state.defs = block;
}

static inline void llvm_pos_code(LlvmBackend *llvm, LLVMBasicBlockRef block) {
    LLVMPositionBuilderAtEnd(llvm->builder, block);
    llvm->state.code = block;
}

LlvmState llvm_switch_state(LlvmBackend *llvm, LlvmState state);
