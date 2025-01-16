#include "../../parts.h"

void llvm_stmt_if(LlvmBackend *llvm, AstIfElse *if_else) {
	LLVMBasicBlockRef if_block = LLVMAppendBasicBlock(llvm->func, "");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(llvm->func, "");
	LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm->func, "");
	LLVMBuildCondBr(llvm->builder, llvm_expr(llvm, &if_else->main.expr), if_block, else_block);
	LLVMPositionBuilderAtEnd(llvm->builder, if_block);
	if (llvm_body(llvm, if_else->main.body)) {
		LLVMBuildBr(llvm->builder, end_block);
	}
	LLVMPositionBuilderAtEnd(llvm->builder, else_block);
	for (size_t i = 0; i < vec_len(if_else->else_ifs); i++) {
		LLVMBasicBlockRef next_else_block = LLVMAppendBasicBlock(llvm->func, "");
		AstIfBody *bl = &if_else->else_ifs[i];
		LLVMBuildCondBr(llvm->builder, llvm_expr(llvm, &bl->expr), if_block, else_block);
		else_block = next_else_block;
		LLVMPositionBuilderAtEnd(llvm->builder, else_block);
	}
	if (!if_else->has_else || llvm_body(llvm, if_else->else_body)) {
		LLVMBuildBr(llvm->builder, end_block);
	}
	LLVMPositionBuilderAtEnd(llvm->builder, end_block);
}
