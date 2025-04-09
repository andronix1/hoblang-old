#include "if.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include "llvm/llvm.h"
#include "llvm/node/body.h"
#include "llvm/node/expr.h"
#include "core/vec.h"

void llvm_if_block(LlvmBackend *llvm, AstIfCondBlock *if_block, LLVMBasicBlockRef end) {
    LLVMBasicBlockRef
        if_body = LLVMAppendBasicBlock(llvm->state.func, ""),
        else_body = LLVMAppendBasicBlock(llvm->state.func, "");
    LLVMBuildCondBr(llvm->builder, llvm_expr_get(llvm, if_block->cond), if_body, else_body);
    llvm_pos_defs(llvm, if_body); // TODO: block defs set in loops
    llvm_pos_code(llvm, if_body);
    llvm_emit_body(llvm, if_block->body);
    LLVMBuildBr(llvm->builder, end);
    llvm_pos_defs(llvm, else_body);
    llvm_pos_code(llvm, else_body);
}

void llvm_if(LlvmBackend *llvm, AstIf *if_else) {
    LLVMBasicBlockRef end = LLVMAppendBasicBlock(llvm->state.func, "");
    llvm_if_block(llvm, if_else->if_block, end);
    for (size_t i = 0; i < vec_len(if_else->else_ifs); i++) {
        llvm_if_block(llvm, if_else->else_ifs[i], end);
    }
    if (if_else->else_body) {
        llvm_emit_body(llvm, if_else->else_body);        
    }
    LLVMBuildBr(llvm->builder, end);
    llvm_pos_defs(llvm, end);
    llvm_pos_code(llvm, end);
}
