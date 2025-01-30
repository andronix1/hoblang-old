#include "../parts.h"
#include "../utils/slices.h"

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
	return LLVMBuildCall2(
		llvm->builder,
		llvm_sema_function_type(&func_call->value.sema_type->func),
		LLVMBuildLoad2(
			llvm->builder,
			llvm_resolve_type(func_call->value.sema_type),
			llvm_value(llvm, &func_call->value),
			""
		),
		params, vec_len(func_call->args),
		""
	);
}

LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr) {
	switch (expr->type) {
		case AST_EXPR_NOT: return LLVMBuildNot(llvm->builder, llvm_expr(llvm, expr->not_expr), "");
		case AST_EXPR_VALUE: return LLVMBuildLoad2(llvm->builder, llvm_resolve_type(expr->sema_type), llvm_value(llvm, &expr->value), "");
		case AST_EXPR_REF: return llvm_value(llvm, &expr->value);
		case AST_EXPR_INTEGER: return LLVMConstInt(llvm_resolve_type(expr->sema_type), expr->integer, false);
		case AST_EXPR_BOOL: return LLVMConstInt(LLVMInt1Type(), expr->boolean, false);
		case AST_EXPR_CHAR: return LLVMConstInt(LLVMInt8Type(), expr->character, false);
		case AST_EXPR_STR: return llvm_slice_from_str(llvm, &expr->str);
		case AST_EXPR_AS: {
			LLVMTypeRef to_type = llvm_resolve_type(expr->as.type.sema);
			LLVMValueRef value = llvm_expr(llvm, expr->as.expr);
			switch (expr->as.conv_type) {
				case SEMA_AS_CONV_EXTEND: return LLVMBuildZExt(llvm->builder, value, to_type, "");
				case SEMA_AS_CONV_TRUNC: return LLVMBuildTrunc(llvm->builder, value, to_type, "");
				case SEMA_AS_CONV_BITCAST: return LLVMBuildBitCast(llvm->builder, value, to_type, "");
				case SEMA_AS_CONV_PTR_TO_INT: return LLVMBuildPtrToInt(llvm->builder, value, to_type, "");
				case SEMA_AS_CONV_INT_TO_PTR: return LLVMBuildIntToPtr(llvm->builder, value, to_type, "");
				case SEMA_AS_CONV_ARR_PTR_TO_SLICE: return llvm_slice_from_array_ptr(
					llvm,
					llvm_resolve_type(expr->as.expr->sema_type->array.of),
					value,
					expr->as.expr->sema_type->array.length
				);
				case SEMA_AS_CONV_SLICE_TO_PTR: return LLVMBuildExtractValue( llvm->builder, value, 1, "");
			}
			break;
		}
		case AST_EXPR_UNARY: {
			switch (expr->unary.type) {
				case AST_UNARY_MINUS: return LLVMBuildNeg(llvm->builder, llvm_expr(llvm, expr->unary.expr), "");
				case AST_UNARY_BITNOT: return LLVMBuildNot(llvm->builder, llvm_expr(llvm, expr->unary.expr), "");
			}
			assert(0, "invalid unary {int}", expr->unary.type);
			return NULL;
		}
		case AST_EXPR_BINOP: {
			LLVMValueRef right = llvm_expr(llvm, expr->binop.right);
			LLVMValueRef left = llvm_expr(llvm, expr->binop.left);
			switch (expr->binop.type) {
				case AST_BINOP_ADD: return LLVMBuildAdd(llvm->builder, left, right, "");
				case AST_BINOP_XOR: return LLVMBuildXor(llvm->builder, left, right, "");
				case AST_BINOP_BITAND: return LLVMBuildAnd(llvm->builder, left, right, "");
				case AST_BINOP_BITOR: return LLVMBuildOr(llvm->builder, left, right, "");
				case AST_BINOP_SHR: return LLVMBuildLShr(llvm->builder, left, right, "");
				case AST_BINOP_SHL: return LLVMBuildShl(llvm->builder, left, right, "");
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
			LLVMTypeRef of = llvm_resolve_type(expr->sema_type->array.of);
			LLVMTypeRef type = LLVMArrayType(of, vec_len(expr->array));
			LLVMValueRef array = LLVMBuildAlloca(llvm->builder, type, "new_arr");
			for (size_t i = 0; i < vec_len(expr->array); i++) {
				LLVMValueRef indices[1] = { LLVMConstInt(LLVMInt32Type(), i, false) };
				LLVMBuildStore(
					llvm->builder,
					llvm_expr(llvm, &expr->array[i]),
					LLVMBuildGEP2(llvm->builder, of, array, indices, 1, "arri")
				);
			}
			
			return LLVMBuildLoad2(llvm->builder, type, array, "");
		}
	}
	assert(0, "invalid expr {int}", expr->type);
	return NULL;
}
