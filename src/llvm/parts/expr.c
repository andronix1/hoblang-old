#include "../parts.h"

bool llvm_is_signed(SemaType *type) {
	return
		type->primitive == PRIMITIVE_I8 || 
		type->primitive == PRIMITIVE_I16 || 
		type->primitive == PRIMITIVE_I32 || 
		type->primitive == PRIMITIVE_I64;
}

LLVMValueRef llvm_func_call(LlvmBackend *llvm, AstFuncCall *func_call) {
	LLVMValueRef *params = malloc(sizeof(LLVMValueRef) * vec_len(func_call->args));
	for (size_t i = 0; i < vec_len(func_call->args); i++) {
		params[i] = llvm_expr(llvm, &func_call->args[i]);
	}
	return LLVMBuildCall2(llvm->builder, llvm_resolve_type_of(llvm, &func_call->name), llvm_resolve_value(llvm, &func_call->name), params, vec_len(func_call->args), "");
}

LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr) {
	switch (expr->type) {
		case AST_EXPR_VALUE: {
			LlvmValue *value = llvm_resolve(llvm, &expr->value);
			if (!value->ptr) {
				return value->value;
			}
			return LLVMBuildLoad2(llvm->builder, value->type, value->value, "");
		}
		case AST_EXPR_INTEGER: return LLVMConstInt(LLVMInt32Type(), expr->integer, true);
		case AST_EXPR_BOOL: return LLVMConstInt(LLVMInt1Type(), expr->integer, false);
		case AST_EXPR_CHAR: return LLVMConstInt(LLVMInt8Type(), expr->integer, false);
		case AST_EXPR_AS: {
			LLVMTypeRef to_type = llvm_resolve_type(expr->as.type.sema);
			return LLVMBuildZExt(llvm->builder, LLVMBuildTrunc(llvm->builder, llvm_expr(llvm, expr->as.expr), to_type, ""), to_type, ""); // TODO: check possibility at sema 
		}
			//case AST_EXPR_AS: return llvm_expr(llvm, expr->as.expr);
		case AST_EXPR_BINOP: {
			LLVMValueRef right = llvm_expr(llvm, expr->binop.right);
			LLVMValueRef left = llvm_expr(llvm, expr->binop.left);
			switch (expr->binop.type) {
				case AST_BINOP_ADD: return LLVMBuildAdd(llvm->builder, left, right, "");
				case AST_BINOP_SUB: return LLVMBuildSub(llvm->builder, left, right, "");
				case AST_BINOP_MUL: return LLVMBuildMul(llvm->builder, left, right, "");
				case AST_BINOP_DIV: return LLVMBuildSDiv(llvm->builder, left, right, "");
				case AST_BINOP_EQ: return LLVMBuildICmp(llvm->builder, LLVMIntEQ, left, right, "");
				case AST_BINOP_NEQ: return LLVMBuildICmp(llvm->builder, LLVMIntNE, left, right, "");
				case AST_BINOP_GT: return LLVMBuildICmp(llvm->builder, LLVMIntSGT, left, right, "");
				case AST_BINOP_GE: return LLVMBuildICmp(llvm->builder, LLVMIntSGE, left, right, "");
				case AST_BINOP_LT: return LLVMBuildICmp(llvm->builder, LLVMIntSLT, left, right, "");
				case AST_BINOP_LE: return LLVMBuildICmp(llvm->builder, LLVMIntSLE, left, right, "");
			}
			assert(0, "invalid binop {int}", expr->binop.type);
			return NULL;
		}
		case AST_EXPR_FUNCALL: return llvm_func_call(llvm, &expr->func_call);
	}
	assert(0, "invalid expr {int}", expr->type);
	return NULL;
}
