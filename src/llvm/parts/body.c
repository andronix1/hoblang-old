#include "../parts.h"

void llvm_body(LlvmBackend *llvm, AstBody *body) {
	llvm_push_scope(llvm);
	for (size_t i = 0; i < vec_len(body->stmts); i++) {
		AstStmt *stmt = &body->stmts[i];
		switch (stmt->type) {
			case AST_STMT_VAR: {
				assert(stmt->var.typed, "var must be typed at the sema stage");
				LLVMTypeRef type = llvm_resolve_type(stmt->var.type.sema);
				LLVMValueRef var = LLVMBuildAlloca(llvm->builder, type, slice_to_cstr(&stmt->var.name));
				LlvmValue value = {
					.type = type,
					.value = var,
					.name = &stmt->var.name,
					.ptr = true
				};
				llvm_push_value(llvm, &value);
				if (stmt->var.initializes) {
					LLVMBuildStore(llvm->builder, llvm_expr(llvm, &stmt->var.expr), var);
				}
				break;
			}
			case AST_STMT_RETURN: {
				if (stmt->ret.has_value) {
					LLVMBuildRet(llvm->builder, llvm_expr(llvm, &stmt->ret.expr));
				} else {
					LLVMBuildRetVoid(llvm->builder);
				}
				break;
			}
			case AST_STMT_FUNC_CALL: {
				llvm_func_call(llvm, &stmt->func_call);
				break;
			}
			case AST_STMT_IF: {
				LLVMBasicBlockRef if_block = LLVMAppendBasicBlock(llvm->func, "");
				LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(llvm->func, "");
				LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm->func, "");
				LLVMValueRef expr = llvm_expr(llvm, &stmt->if_else.main.expr);
				LLVMBuildCondBr(llvm->builder, expr, if_block, else_block);
				LLVMPositionBuilderAtEnd(llvm->builder, if_block);
				llvm_body(llvm, stmt->if_else.main.body);
				LLVMBuildBr(llvm->builder, end_block);
				assert(vec_len(stmt->if_else.else_ifs) == 0, "if else NYI");
				LLVMPositionBuilderAtEnd(llvm->builder, else_block);
				if (stmt->if_else.has_else) {
					llvm_body(llvm, stmt->if_else.else_body);
				}
				LLVMBuildBr(llvm->builder, end_block);
				LLVMPositionBuilderAtEnd(llvm->builder, end_block);
				break;
			}
		}
	}
	llvm_pop_scope(llvm);
}
