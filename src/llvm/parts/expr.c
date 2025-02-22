#include "llvm/private.h"
#include "llvm/parts/expr.h"
#include "llvm/parts/type.h"
#include "llvm/parts/path.h"
#include "llvm/parts/types/slice.h"
#include "llvm/parts/types/optional.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include "sema/value/private.h"
#include "sema/module/decls/impl.h"
#include "core/vec.h"
#include "ast/private/expr.h"

LLVMValueRef llvm_call(LlvmBackend *llvm, AstCall *call) {
	LLVMValueRef callable = llvm_expr(llvm, call->callable, true);
	LLVMValueRef ext_base = call->callable->value->type == SEMA_VALUE_EXT_FUNC_HANDLE ?
		call->callable->value->ext_func_handle : NULL;
	size_t ext_offset = ext_base != NULL;
	LLVMValueRef *params = alloca(sizeof(LLVMValueRef) * (vec_len(call->args) + ext_offset));
	if (ext_base) {
		params[0] = ext_base;
	}
	for (size_t i = 0; i < vec_len(call->args); i++) {
		params[i + ext_offset] = llvm_expr(llvm, call->args[i], true);
	}
	SemaType *returning = call->callable->value->sema_type->func.returning;
	bool is_void = sema_types_equals(returning, sema_type_primitive_void());
	return LLVMBuildCall2(
		llvm_builder(llvm),
		llvm_sema_function_type(&call->callable->value->sema_type->func),
		callable,
		params, vec_len(call->args) + ext_offset,
		is_void ? "" : "call_result"
	);
}

LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr, bool load) {
	switch (expr->type) {
		case AST_EXPR_UNWRAP: {
            LLVMValueRef opt = llvm_expr(llvm, expr->unwrap.expr, true);
            LLVMValueRef is_null = LLVMBuildNot(
                llvm_builder(llvm),
                llvm_opt_is_null(
                    llvm,
                    llvm_resolve_type(expr->unwrap.expr->value->sema_type),
                    opt,
                    true
                ),
                "is_not_null"
            );
            expr->unwrap.decl->llvm_value = llvm_opt_value(
                llvm,
                llvm_resolve_type(expr->unwrap.expr->value->sema_type->optional_of),
                opt,
                true
            );
            return is_null;
        }
		case AST_EXPR_GET_INNER_PATH: {
			LLVMValueRef value = llvm_resolve_inner_path(
				llvm,
				llvm_expr(llvm, expr->get_inner.of, false),
				&expr->get_inner.path,
				expr->value
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
            LLVMValueRef value = llvm_resolve_path(llvm, &expr->get_local.path, expr->value);
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
		case AST_EXPR_NULL: {
            switch (expr->null_type) {
                case SEMA_NULL_POINTER:
                    return LLVMConstNull(llvm_resolve_type(expr->value->sema_type));
                case SEMA_NULL_OPTIONAL:
                    return llvm_opt_null(llvm, llvm_resolve_type(expr->value->sema_type->optional_of));
            }
            assert(0, "invalid sema null type");
        }
		case AST_EXPR_NOT: return LLVMBuildNot(llvm_builder(llvm), llvm_expr(llvm, expr->not_expr, true), "");
		case AST_EXPR_REF: return llvm_expr(llvm, expr->ref_expr, false);
		case AST_EXPR_INTEGER: return LLVMConstInt(llvm_resolve_type(expr->value->sema_type), expr->integer, false);
		case AST_EXPR_FLOAT: return LLVMConstReal(llvm_resolve_type(expr->value->sema_type), expr->float_value);
		case AST_EXPR_BOOL: return LLVMConstInt(LLVMInt1Type(), expr->boolean, false);
		case AST_EXPR_CHAR: return LLVMConstInt(LLVMInt8Type(), expr->character, false);
		case AST_EXPR_STR: return llvm_slice_from_str(llvm, &expr->str);
        case AST_EXPR_RET_ON_NULL: {
            LLVMBasicBlockRef on_null = LLVMAppendBasicBlock(
                llvm_current_func(llvm),
                "on_null"
            );
            LLVMBasicBlockRef on_not_null = LLVMAppendBasicBlock(
                llvm_current_func(llvm),
                "on_not_null"
            );
            LLVMValueRef opt = llvm_expr(llvm, expr->ret_on_null.expr, false);
            LLVMBuildCondBr(
                llvm_builder(llvm),
                llvm_opt_is_null(
                    llvm,
                    llvm_resolve_type(expr->ret_on_null.expr->value->sema_type),
                    opt,
                    true
                ),
                on_null,
                on_not_null
            );
            LLVMPositionBuilderAtEnd(llvm_builder(llvm), on_null);
            LLVMBuildRet(
                llvm_builder(llvm),
                llvm_opt_null(
                    llvm,
                    llvm_resolve_type(expr->ret_on_null.fret->optional_of)
                )
            );
            LLVMPositionBuilderAtEnd(llvm_builder(llvm), on_not_null);
            llvm_set_code_block(llvm, on_not_null);
            return llvm_opt_value(
                llvm,
                llvm_resolve_type(expr->value->sema_type),
                opt,
                false
            );
        }
		case AST_EXPR_IDX: {
			switch (expr->idx.sema) {
				case SEMA_EXPR_IDX_SLICE: {
					LLVMValueRef indices[] = {
						llvm_expr(llvm, expr->idx.idx, true)
					};
					LLVMValueRef value = LLVMBuildGEP2(
						llvm_builder(llvm),
						llvm_resolve_type(expr->value->sema_type),
                        llvm_slice_ptr(
                            llvm,
                            llvm_resolve_type(expr->idx.of->value->sema_type->slice_of),
                            llvm_expr(llvm, expr->idx.of, false),
                            true
                        ),
						indices, 1,
						"idx_element_ptr"
					);
					if (load) {
						return LLVMBuildLoad2(
							llvm_builder(llvm),
							llvm_resolve_type(expr->value->sema_type),
							value, 
							"loaded_idx"
						);
					}
					return value;
				}
				case SEMA_EXPR_IDX_ARRAY: {
					LLVMValueRef indices[] = {
						llvm_expr(llvm, expr->idx.idx, true)
					};
					LLVMValueRef value = LLVMBuildGEP2(
						llvm_builder(llvm),
						llvm_resolve_type(expr->value->sema_type),
                        llvm_expr(llvm, expr->idx.of, false),
						indices, 1,
						"idx_element_ptr"
					);
					if (load) {
						return LLVMBuildLoad2(
							llvm_builder(llvm),
							llvm_resolve_type(expr->value->sema_type),
							value, 
							"loaded_idx"
						);
					}
					return value;
				}
			}
			break;
		}
		case AST_EXPR_AS: {
			LLVMTypeRef to_type = llvm_resolve_type(expr->as.sema_type);
			LLVMValueRef value = llvm_expr(llvm, expr->as.expr, true);
			switch (expr->as.conv_type) {
				case SEMA_AS_CONV_EXTEND: return LLVMBuildZExt(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_TRUNC: return LLVMBuildTrunc(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_FEXTEND: return LLVMBuildFPExt(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_FTRUNC: return LLVMBuildFPTrunc(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_BITCAST: return LLVMBuildBitCast(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_PTR_TO_INT: return LLVMBuildPtrToInt(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_INT_TO_PTR: return LLVMBuildIntToPtr(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_INT_TO_FLOAT: return LLVMBuildSIToFP(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_UINT_TO_FLOAT: return LLVMBuildUIToFP(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_FLOAT_TO_INT: return LLVMBuildFPToSI(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_FLOAT_TO_UINT: return LLVMBuildFPToUI(llvm_builder(llvm), value, to_type, "");
				case SEMA_AS_CONV_ARR_PTR_TO_SLICE: return llvm_slice_from_array_ptr(
					llvm,
					llvm_resolve_type(expr->as.expr->value->sema_type->array.of),
					value,
					expr->as.expr->value->sema_type->ptr_to->array.length
				);
				case SEMA_AS_CONV_SLICE_TO_PTR: return LLVMBuildExtractValue(llvm_builder(llvm), value, 1, "");
				case SEMA_AS_CONV_OPT_WRAP: return llvm_opt_wrap(llvm, llvm_resolve_type(expr->as.expr->value->sema_type), value);
				case SEMA_AS_CONV_OPT_UNWRAP: {
                    value = llvm_opt_value(llvm, llvm_resolve_type(expr->as.expr->value->sema_type), value, load);
                    return value;
                }
				case SEMA_AS_CONV_IGNORE: return value;
			}
			break;
		}
		case AST_EXPR_UNARY: {
			bool is_float = sema_type_is_float(expr->unary.expr->value->sema_type);
			switch (expr->unary.type) {
				case AST_UNARY_MINUS: return (is_float ? LLVMBuildFNeg : LLVMBuildNeg)(llvm_builder(llvm), llvm_expr(llvm, expr->unary.expr, true), "");
				case AST_UNARY_BITNOT: return LLVMBuildNot(llvm_builder(llvm), llvm_expr(llvm, expr->unary.expr, true), "");
			}
			assert(0, "invalid unary {int}", expr->unary.type);
			return NULL;
		}
		case AST_EXPR_BINOP: {
			bool is_float = sema_type_is_float(expr->binop.left->value->sema_type);
			LLVMValueRef right = llvm_expr(llvm, expr->binop.right, true);
			LLVMValueRef left = llvm_expr(llvm, expr->binop.left, true);
			switch (expr->binop.type) {
				case AST_BINOP_MOD: return LLVMBuildURem(llvm_builder(llvm), left, right, "");
				case AST_BINOP_ADD: return (is_float ? LLVMBuildFAdd : LLVMBuildAdd)(llvm_builder(llvm), left, right, "");
				case AST_BINOP_SUB: return (is_float ? LLVMBuildFSub : LLVMBuildSub)(llvm_builder(llvm), left, right, "");
				case AST_BINOP_MUL: return (is_float ? LLVMBuildFMul : LLVMBuildMul)(llvm_builder(llvm), left, right, "");
				case AST_BINOP_DIV: return (is_float ? LLVMBuildFDiv : LLVMBuildSDiv)(llvm_builder(llvm), left, right, "");
				case AST_BINOP_XOR: return LLVMBuildXor(llvm_builder(llvm), left, right, "");
				case AST_BINOP_BITAND: return LLVMBuildAnd(llvm_builder(llvm), left, right, "");
				case AST_BINOP_BITOR: return LLVMBuildOr(llvm_builder(llvm), left, right, "");
				case AST_BINOP_SHR: return LLVMBuildLShr(llvm_builder(llvm), left, right, "");
				case AST_BINOP_SHL: return LLVMBuildShl(llvm_builder(llvm), left, right, "");
				#define CMP(itype, ftype) (is_float ? \
					LLVMBuildFCmp(llvm_builder(llvm), ftype, left, right, "") : \
					LLVMBuildICmp(llvm_builder(llvm), itype, left, right, ""))
				case AST_BINOP_EQ: return CMP(LLVMIntEQ, LLVMRealOEQ);
				case AST_BINOP_NEQ: return CMP(LLVMIntNE, LLVMRealONE);
				case AST_BINOP_GT: return CMP(LLVMIntSGT, LLVMRealOGT);
				case AST_BINOP_GE: return CMP(LLVMIntSGE, LLVMRealOGE);
				case AST_BINOP_LT: return CMP(LLVMIntSLT, LLVMRealOLT);
				case AST_BINOP_LE: return CMP(LLVMIntSLE, LLVMRealOLE);
				case AST_BINOP_AND: return LLVMBuildAnd(llvm_builder(llvm), left, right, "");
				case AST_BINOP_OR: return LLVMBuildOr(llvm_builder(llvm), left, right, "");
			}
			assert(0, "invalid binop {int}", expr->binop.type);
			return NULL;
		}
		case AST_EXPR_CALL: {
			return llvm_call(llvm, &expr->call);
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
