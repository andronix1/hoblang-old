#include "llvm/private.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/type.h"
#include "llvm/parts/path.h"
#include "llvm/parts/types/slice.h"
#include "sema/value/private.h"
#include "core/vec.h"
#include "ast/private/expr.h"

LLVMValueRef llvm_call(LlvmBackend *llvm, AstCall *call) {
	LLVMValueRef *params = alloca(sizeof(LLVMValueRef) * vec_len(call->args));
	for (size_t i = 0; i < vec_len(call->args); i++) {
		params[i] = llvm_expr(llvm, call->args[i], true);
	}
	SemaType *returning = call->callable->value->sema_type->func.returning;
	bool is_void = returning->type == SEMA_TYPE_PRIMITIVE && returning->primitive == PRIMITIVE_VOID;
	LLVMValueRef call_result = LLVMBuildCall2(
		llvm_builder(llvm),
		llvm_sema_function_type(&call->callable->value->sema_type->func),
		llvm_expr(llvm, call->callable, true),
		params, vec_len(call->args),
		is_void ? "" : "call_result"
	);
	if (is_void) {
		return call_result;
	}
	LLVMValueRef value = LLVMBuildAlloca(
		llvm_builder(llvm),
		llvm_resolve_type(returning),
		"alloca_call_result"
	);
	LLVMBuildStore(
		llvm_builder(llvm), 
		call_result,
		value
	);
	return value;
}
/*
LLVMValueRef llvm_get_local_value_path(LlvmBackend *llvm, AstPath *path) {
	LLVMValueRef result = NULL;
	for (size_t i = 0; i < vec_len(path->segments); i++) {
		SemaPathSegment *segment = &path->segments[i].sema;
		switch (segment->type) {
			case SEMA_PATH_SEGMENT_MODULE:
			case SEMA_PATH_SEGMENT_TYPE: 
				break;
			case SEMA_PATH_SEGMENT_DECL:
				result = segment->decl->value_decl.llvm_value;
				printf("GET(llvm) %p\n", &segment->decl->value_decl);
				break;
			case SEMA_PATH_SEGMENT_STRUCT_MEMBER: {
				LLVMValueRef indices[2] = {
					LLVMConstInt(LLVMInt32Type(), 0, false),
					LLVMConstInt(LLVMInt32Type(), segment->struct_member.member_id, false)
				};
				result = LLVMBuildGEP2(
					llvm_builder(llvm),
					llvm_resolve_type(segment->struct_member.struct_type),
					result,
					indices, 2,
					"struct_member"
				);
				break;
			}
			case SEMA_PATH_SEGMENT_SLICE_LENGTH:
				result = llvm_slice_len(
					llvm,
					llvm_resolve_type(segment->slice_type),
					result
				);
				break;
			case SEMA_PATH_SEGMENT_SLICE_PTR:
				result = llvm_slice_ptr(
					llvm,
					llvm_resolve_type(segment->slice_type),
					result
				);
				break;
		}
	}
	assert(result, "path `{ast::path}` was not resolved!", path);
	return result;
}
*/
LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr, bool load) {
	switch (expr->type) {
		case AST_EXPR_GET_INNER_PATH: {
			LLVMValueRef allocated_expr = LLVMBuildAlloca(
				llvm_builder(llvm),
				llvm_resolve_type(expr->value->sema_type),
				"allocated_expr"
			);

			LLVMBuildStore(
				llvm_builder(llvm),
				llvm_expr(llvm, expr->get_inner.of, true),
				allocated_expr
			);
			// str_ptr: *void
			// EXPR: (str_ptr as *u8).*

			// **void alloca_src_ptr
			// *u8 src_ptr_as_u8
			// *u8 src_ptr_as_u8
			LLVMValueRef value = llvm_resolve_inner_path(
				llvm,
				allocated_expr, //llvm_expr(llvm, expr->get_inner.of, false),
				&expr->get_inner.path
			);
			if (load && expr->value->type == SEMA_VALUE_VAR) {
                return LLVMBuildLoad2(
                    llvm_builder(llvm),
                    llvm_resolve_type(expr->value->sema_type),
                    value,
                    ""
                );
            }
			return value;
		}
		case AST_EXPR_GET_LOCAL_PATH: {
            LLVMValueRef value = llvm_resolve_path(llvm, &expr->get_local.path);
            if (load && expr->value->type == SEMA_VALUE_VAR) {
                return LLVMBuildLoad2(
                    llvm_builder(llvm),
                    llvm_resolve_type(expr->value->sema_type),
                    value,
                    ""
                );
            }
            return value;
        }
		case AST_EXPR_NOT: return LLVMBuildNot(llvm_builder(llvm), llvm_expr(llvm, expr->not_expr, true), "");
		case AST_EXPR_REF: return llvm_expr(llvm, expr->ref_expr, false);
		case AST_EXPR_INTEGER: return LLVMConstInt(llvm_resolve_type(expr->value->sema_type), expr->integer, false);
		case AST_EXPR_BOOL: return LLVMConstInt(LLVMInt1Type(), expr->boolean, false);
		case AST_EXPR_CHAR: return LLVMConstInt(LLVMInt8Type(), expr->character, false);
		case AST_EXPR_STR: return llvm_slice_from_str(llvm, &expr->str);
		case AST_EXPR_AS: {
			LLVMTypeRef to_type = llvm_resolve_type(expr->as.type.sema);
			LLVMValueRef value = llvm_expr(llvm, expr->as.expr, true);
			switch (expr->as.conv_type) {
				case SEMA_AS_CONV_EXTEND: return LLVMBuildZExt(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_TRUNC: return LLVMBuildTrunc(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_BITCAST: return LLVMBuildBitCast(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_PTR_TO_INT: return LLVMBuildPtrToInt(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_INT_TO_PTR: return LLVMBuildIntToPtr(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_ARR_PTR_TO_SLICE: return llvm_slice_from_array_ptr(
					llvm,
					llvm_resolve_type(expr->as.expr->value->sema_type->array.of),
					value,
					expr->as.expr->value->sema_type->ptr_to->array.length
				);
				case SEMA_AS_CONV_SLICE_TO_PTR: return LLVMBuildExtractValue(llvm_builder(llvm), value, 1, "");
				case SEMA_AS_CONV_IGNORE: return value;
			}
			break;
		}
		case AST_EXPR_UNARY: {
			switch (expr->unary.type) {
				case AST_UNARY_MINUS: return LLVMBuildNeg(llvm_builder(llvm), llvm_expr(llvm, expr->unary.expr, true), "");
				case AST_UNARY_BITNOT: return LLVMBuildNot(llvm_builder(llvm), llvm_expr(llvm, expr->unary.expr, true), "");
			}
			assert(0, "invalid unary {int}", expr->unary.type);
			return NULL;
		}
		case AST_EXPR_BINOP: {
			LLVMValueRef right = llvm_expr(llvm, expr->binop.right, true);
			LLVMValueRef left = llvm_expr(llvm, expr->binop.left, true);
			switch (expr->binop.type) {
				case AST_BINOP_ADD: return LLVMBuildAdd(llvm_builder(llvm), left, right, "");
				case AST_BINOP_XOR: return LLVMBuildXor(llvm_builder(llvm), left, right, "");
				case AST_BINOP_BITAND: return LLVMBuildAnd(llvm_builder(llvm), left, right, "");
				case AST_BINOP_BITOR: return LLVMBuildOr(llvm_builder(llvm), left, right, "");
				case AST_BINOP_SHR: return LLVMBuildLShr(llvm_builder(llvm), left, right, "");
				case AST_BINOP_SHL: return LLVMBuildShl(llvm_builder(llvm), left, right, "");
				case AST_BINOP_SUB: return LLVMBuildSub(llvm_builder(llvm), left, right, "");
				case AST_BINOP_MUL: return LLVMBuildMul(llvm_builder(llvm), left, right, "");
				case AST_BINOP_DIV: return LLVMBuildSDiv(llvm_builder(llvm), left, right, "");
				case AST_BINOP_EQ: return LLVMBuildICmp(llvm_builder(llvm), LLVMIntEQ, left, right, "");
				case AST_BINOP_NEQ: return LLVMBuildICmp(llvm_builder(llvm), LLVMIntNE, left, right, "");
				case AST_BINOP_GT: return LLVMBuildICmp(llvm_builder(llvm), LLVMIntSGT, left, right, "");
				case AST_BINOP_GE: return LLVMBuildICmp(llvm_builder(llvm), LLVMIntSGE, left, right, "");
				case AST_BINOP_LT: return LLVMBuildICmp(llvm_builder(llvm), LLVMIntSLT, left, right, "");
				case AST_BINOP_LE: return LLVMBuildICmp(llvm_builder(llvm), LLVMIntSLE, left, right, "");
				case AST_BINOP_AND: return LLVMBuildAnd(llvm_builder(llvm), left, right, "");
				case AST_BINOP_OR: return LLVMBuildOr(llvm_builder(llvm), left, right, "");
			}
			assert(0, "invalid binop {int}", expr->binop.type);
			return NULL;
		}
		case AST_EXPR_CALL: {
			LLVMValueRef value = llvm_call(llvm, &expr->call);
			if (load) {
				return LLVMBuildLoad2(
					llvm_builder(llvm),
					llvm_resolve_type(expr->value->sema_type),
					value,
					""
				);
			}
			return value;
		}
		case AST_EXPR_ARRAY: {
			LLVMTypeRef of = llvm_resolve_type(expr->value->sema_type->array.of);
			LLVMTypeRef type = LLVMArrayType(of, vec_len(expr->array));
			LLVMValueRef array = LLVMBuildAlloca(llvm_builder(llvm), type, "new_arr");
			for (size_t i = 0; i < vec_len(expr->array); i++) {
				LLVMValueRef indices[1] = { LLVMConstInt(LLVMInt32Type(), i, false) };
				LLVMBuildStore(
					llvm_builder(llvm),
					llvm_expr(llvm, expr->array[i], true),
					LLVMBuildGEP2(llvm_builder(llvm), of, array, indices, 1, "arri")
				);
			}
			
			return LLVMBuildLoad2(llvm_builder(llvm), type, array, "");
		}
	}
	assert(0, "invalid expr {int}", expr->type);
	return NULL;
}
