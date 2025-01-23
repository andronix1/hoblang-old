#include "../parts.h"

bool llvm_is_signed(SemaType *type) {
	return
		type->primitive == PRIMITIVE_I8 || 
		type->primitive == PRIMITIVE_I16 || 
		type->primitive == PRIMITIVE_I32 || 
		type->primitive == PRIMITIVE_I64;
}

LLVMValueRef llvm_func_call(LlvmBackend *llvm, AstFuncCall *func_call) {
	LLVMValueRef *params = alloca(sizeof(LLVMValueRef) * vec_len(func_call->args));
	for (size_t i = 0; i < vec_len(func_call->args); i++) {
		params[i] = llvm_expr(llvm, &func_call->args[i]);
	}
	return LLVMBuildCall2(llvm->builder, llvm_resolve_type(func_call->value.sema_type), llvm_value(llvm, &func_call->value), params, vec_len(func_call->args), "");
}

LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr) {
	switch (expr->type) {
		case AST_EXPR_NOT: return LLVMBuildNot(llvm->builder, llvm_expr(llvm, expr->not_expr), "");
		case AST_EXPR_VALUE: return LLVMBuildLoad2(llvm->builder, llvm_resolve_type(expr->sema_type), llvm_value(llvm, &expr->value), "");
		case AST_EXPR_REF: return llvm_value(llvm, &expr->value);
		case AST_EXPR_INTEGER: return LLVMConstInt(LLVMInt32Type(), expr->integer, false);
		case AST_EXPR_BOOL: return LLVMConstInt(LLVMInt1Type(), expr->boolean, false);
		case AST_EXPR_CHAR: return LLVMConstInt(LLVMInt8Type(), expr->character, false);
		case AST_EXPR_STR: {
			
			LLVMTypeRef type = LLVMArrayType(LLVMInt8Type(), expr->str.len);
			LLVMValueRef value = LLVMAddGlobal(llvm->module, type, ""); //LLVMBuildAlloca(llvm->builder, type, "");
			LLVMSetInitializer(value, LLVMConstString(expr->str.str, expr->str.len, true));
			return LLVMBuildBitCast(llvm->builder, value, LLVMPointerType(LLVMInt8Type(), 0), "");
		}
		case AST_EXPR_AS: {
			LLVMTypeRef to_type = llvm_resolve_type(expr->as.type.sema);
			LLVMValueRef value = llvm_expr(llvm, expr->as.expr);
			int level[] = {
				[PRIMITIVE_I8] = 1,
				[PRIMITIVE_U8] = 1,
				[PRIMITIVE_I16] = 2,
				[PRIMITIVE_U16] = 2,
				[PRIMITIVE_I32] = 3,
				[PRIMITIVE_U32] = 3,
				[PRIMITIVE_I64] = 4,
				[PRIMITIVE_U64] = 4,
				[PRIMITIVE_BOOL] = 0,
			};
			if (expr->as.expr->sema_type->type == SEMA_TYPE_POINTER
					&& expr->as.type.sema->type == SEMA_TYPE_PRIMITIVE) {
				return LLVMBuildPtrToInt(llvm->builder, value, to_type, "");
			}
			if (expr->as.expr->sema_type->type == SEMA_TYPE_PRIMITIVE
					&& expr->as.type.sema->type == SEMA_TYPE_POINTER) {
				return LLVMBuildIntToPtr(llvm->builder, value, to_type, "");
			}
			if (expr->sema_type->type == SEMA_TYPE_PRIMITIVE) {
				if (level[expr->as.expr->sema_type->primitive] < level[expr->sema_type->primitive]) {
					return LLVMBuildZExt(llvm->builder, value, to_type, "");
				}
				return LLVMBuildTrunc(llvm->builder, value, to_type, ""); // TODO: check possibility at sema 
			}
			return LLVMBuildBitCast(llvm->builder, value, to_type, "");
		}
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
				case AST_BINOP_AND: return LLVMBuildAnd(llvm->builder, left, right, "");
				case AST_BINOP_OR: return LLVMBuildOr(llvm->builder, left, right, "");
			}
			assert(0, "invalid binop {int}", expr->binop.type);
			return NULL;
		}
		case AST_EXPR_FUNCALL: return llvm_func_call(llvm, &expr->func_call);	
		case AST_EXPR_ARRAY: {
			LLVMTypeRef ptr_to = llvm_resolve_type(expr->sema_type->ptr_to);
			LLVMTypeRef type = LLVMArrayType(ptr_to, vec_len(expr->array));
			LLVMValueRef value = LLVMBuildAlloca(llvm->builder, type, "");
			LLVMValueRef *vals = alloca(sizeof(LLVMValueRef) * vec_len(expr->array));
			for (size_t i = 0; i < vec_len(expr->array); i++) {
				vals[i] = llvm_expr(llvm, &expr->array[i]);
			}
			LLVMBuildStore(llvm->builder, LLVMConstArray(ptr_to, vals, vec_len(expr->array)), value);
			return LLVMBuildBitCast(llvm->builder, value, LLVMPointerType(ptr_to, 0), "");
		}
	}
	assert(0, "invalid expr {int}", expr->type);
	return NULL;
}
