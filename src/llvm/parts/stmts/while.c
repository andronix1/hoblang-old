#include "llvm/private.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/body.h"
#include "ast/private/stmts/while.h"
#include "sema/module/loop/private.h"

void llvm_stmt_while(LlvmBackend *llvm, AstWhile *while_loop) {
	LLVMBasicBlockRef cond_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "while_cond");
	LLVMBasicBlockRef body_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "while_body");
	LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "while.end");
    while_loop->loop->llvm.continue_block = cond_block;
    while_loop->loop->llvm.break_block = end_block;
	LLVMBuildBr(llvm_builder(llvm), cond_block);
    llvm_set_code_block(llvm, cond_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), cond_block);
	LLVMBuildCondBr(llvm_builder(llvm), llvm_expr(llvm, while_loop->expr, true), body_block, end_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), body_block);
    llvm_set_code_block(llvm, body_block);
	if (llvm_body(llvm, while_loop->body)) {
		LLVMBuildBr(llvm_builder(llvm), cond_block);
	}
    llvm_set_code_block(llvm, end_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), end_block);
}
