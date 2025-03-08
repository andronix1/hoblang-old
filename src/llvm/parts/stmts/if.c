#include "llvm/private.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/body.h"
#include "core/vec.h"
#include "ast/private/stmts/if_else.h"

bool llvm_stmt_if(LlvmBackend *llvm, AstIfElse *if_else) {
	LLVMBasicBlockRef if_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "if.start");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "if.else");
	LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "if.end");
	LLVMBuildCondBr(llvm_builder(llvm), llvm_expr(llvm, if_else->main.expr, true), if_block, else_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), if_block);
    llvm_set_code_block(llvm, if_block);
	if (llvm_body(llvm, if_else->main.body)) {
		LLVMBuildBr(llvm_builder(llvm), end_block);
	}
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), else_block);
    llvm_set_code_block(llvm, else_block);
	for (size_t i = 0; i < vec_len(if_else->else_ifs); i++) {
		LLVMBasicBlockRef next_else_block = LLVMAppendBasicBlock(llvm_current_func(llvm), "");
		LLVMBasicBlockRef else_if_body = LLVMAppendBasicBlock(llvm_current_func(llvm), "");
		AstIfBody *bl = &if_else->else_ifs[i];
		LLVMBuildCondBr(llvm_builder(llvm), llvm_expr(llvm, bl->expr, true), else_if_body, next_else_block);
        llvm_set_code_block(llvm, else_if_body);
		LLVMPositionBuilderAtEnd(llvm_builder(llvm), else_if_body);
		if (llvm_body(llvm, bl->body)) {
			LLVMBuildBr(llvm_builder(llvm), end_block);
		}
		else_block = next_else_block;
        llvm_set_code_block(llvm, else_block);
		LLVMPositionBuilderAtEnd(llvm_builder(llvm), else_block);
	}
    llvm_set_code_block(llvm, else_block);
	if (!if_else->has_else || llvm_body(llvm, if_else->else_body)) {
		LLVMBuildBr(llvm_builder(llvm), end_block);
	}
    llvm_set_code_block(llvm, end_block);
	LLVMPositionBuilderAtEnd(llvm_builder(llvm), end_block);
    return true;
}
