#include "llvm/private.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/body.h"
#include "ast/private/stmts/while.h"

void llvm_stmt_while(LlvmBackend *llvm, AstWhile *while_loop) {
	LLVMBasicBlockRef cond_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "");
	LLVMBasicBlockRef body_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "");
	LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "");
	LLVMBuildBr(llvm_builder(llvm), cond_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), cond_block);
	LLVMBuildCondBr(llvm_builder(llvm), llvm_expr(llvm, while_loop->expr, true), body_block, end_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), body_block);
	if (llvm_body(llvm, while_loop->body)) {
		LLVMBuildBr(llvm_builder(llvm), cond_block);
	}
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), end_block);
}
