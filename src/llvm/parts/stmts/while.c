#include "../../parts.h"
#include "ast/private/stmts/while.h"

void llvm_stmt_while(LlvmBackend *llvm, AstWhile *while_loop) {
	LLVMBasicBlockRef cond_block = LLVMAppendBasicBlock(llvm->func, "");
	LLVMBasicBlockRef body_block = LLVMAppendBasicBlock(llvm->func, "");
	LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm->func, "");
	LLVMBuildBr(llvm->builder, cond_block);
	LLVMPositionBuilderAtEnd(llvm->builder, cond_block);
	LLVMBuildCondBr(llvm->builder, llvm_expr(llvm, while_loop->expr, true), body_block, end_block);
	LLVMPositionBuilderAtEnd(llvm->builder, body_block);
	if (llvm_body(llvm, while_loop->body)) {
		LLVMBuildBr(llvm->builder, cond_block);
	}
	LLVMPositionBuilderAtEnd(llvm->builder, end_block);
}
