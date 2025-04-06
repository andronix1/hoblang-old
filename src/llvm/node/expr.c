#include "expr.h"
#include "ast/shared/expr.h"
#include "ast/shared/path.h"
#include "core/vec.h"
#include "sema/const.h"
#include "core/assert.h"
#include "sema/interface/value.h"
#include "llvm/node/path.h"
#include "llvm/slice.h"
#include "llvm/type.h"
#include "llvm/llvm.h"
#include "llvm/value.h"
#include "sema/type/type.h"
#include <alloca.h>
#include <llvm-c/Core.h>

LLVMValueRef llvm_expr(LlvmBackend *llvm, AstExpr *expr) {
    SemaConst *constant = sema_value_is_const(expr->sema.value);
    if (constant) {
        return llvm_const(llvm, constant);
    }
    switch (expr->kind) {
        case AST_EXPR_CALL: {
            LLVMValueRef callable = llvm_expr_get(llvm, expr->call.callable);
            LLVMValueRef ext_func_handle = expr->call.callable->llvm.ext_func_handle;
            bool is_ext = sema_value_is_ext_func_handle(expr->call.callable->sema.value);
            size_t args_offset = is_ext;
            LLVMValueRef *args = alloca(sizeof(LLVMValueRef) * (vec_len(expr->call.args) + args_offset));
            if (is_ext) {
                args[0] = ext_func_handle;
            }
            for (size_t i = 0; i < vec_len(expr->call.args); i++) {
                args[i + args_offset] = llvm_expr_get(llvm, expr->call.args[i]);
            }
            return LLVMBuildCall2(
                llvm->builder,
                llvm_type(sema_value_is_runtime(expr->call.callable->sema.value)),
                callable,
                args,
                vec_len(expr->call.args) + args_offset,
                ""
            );
        }
        case AST_EXPR_LOCAL_PATH: {
            LLVMValueRef val = llvm_path(llvm, expr->local_path);
            expr->llvm.ext_func_handle = expr->local_path->llvm.ext_handle;
            return val;
        }
        case AST_EXPR_INNER_PATH: {
            LLVMValueRef val = llvm_inner_path(llvm, expr->inner_path.path, llvm_expr(llvm, expr->inner_path.inner));
            expr->llvm.ext_func_handle = expr->inner_path.path->llvm.ext_handle;
            return val;
        }
        case AST_EXPR_SCOPE:
            return llvm_expr(llvm, expr->scope);
        case AST_EXPR_STR: return llvm_slice_from_string(llvm, &expr->str);
        case AST_EXPR_AS: {
            LLVMValueRef inner = llvm_expr_get(llvm, expr->as.inner);
            LLVMTypeRef target = llvm_type(sema_value_is_runtime(expr->sema.value));
            switch (expr->as.sema.kind) {
                case SEMA_AS_IGNORE: return inner;
                case SEMA_AS_PTRCAST: return LLVMBuildBitCast(llvm->builder, inner, target, "");
                case SEMA_AS_INT_CAST: return LLVMBuildIntCast(llvm->builder, inner, target, "");
                case SEMA_AS_PTR_TO_INT: return LLVMBuildPtrToInt(llvm->builder, inner, target, "");
                case SEMA_AS_INT_TO_PTR: return LLVMBuildIntToPtr(llvm->builder, inner, target, "");
                case SEMA_AS_ARR_TO_SLICE: {
                    LLVMValueRef indices[] = {
                        LLVMConstInt(LLVMInt32Type(), 0, false), // TODO: is 64-bit allowed??
                        LLVMConstInt(LLVMInt32Type(), 0, false), // TODO: is 64-bit allowed??
                    };
                    LLVMValueRef array_ptr = LLVMBuildGEP2(
                        llvm->builder,
                        llvm_type(sema_value_is_runtime(expr->as.inner->sema.value)->pointer_to),
                        inner,
                        indices, 2,
                        ""
                    );
                    return llvm_slice_new(
                        llvm,
                        sema_value_is_runtime(expr->sema.value),
                        array_ptr,
                        sema_value_is_runtime(expr->as.inner->sema.value)
                            ->pointer_to->array.length
                    );
                }
                case SEMA_AS_OPT_UNWRAP:
                case SEMA_AS_INT_TO_FLOAT:
                case SEMA_AS_FLOAT_CAST:
                case SEMA_AS_FLOAT_TO_INT:
                    assert(0, "as N(fully)IY");
            }
            assert(0, "invalid as-expression sema kind");
            break;
        }
        case AST_EXPR_UNARY:
            switch (expr->unary.kind) {
                case AST_EXPR_UNARY_REF: return llvm_expr(llvm, expr->unary.inner);
                case AST_EXPR_UNARY_BOOL:
                    switch (expr->unary.boolean) {
                        case AST_EXPR_UNARY_BOOL_NOT:
                            return LLVMBuildNot(
                                llvm->builder,
                                llvm_expr_get(llvm, expr->unary.inner),
                                ""
                            );
                    }
                    assert(0, "invalid unary-bool-expression");
                case AST_EXPR_UNARY_ARITHMETIC:
                  break;
            }
            assert(0, "invalid unary-expression");
        case AST_EXPR_ARRAY:
        case AST_EXPR_STRUCTURE:
        case AST_EXPR_ANON_FUNC:
        case AST_EXPR_IDX:
        case AST_EXPR_RET_ON_NULL:
        case AST_EXPR_UNWRAP:
            assert(0, "NIY");
        case AST_EXPR_BINOP: {
            LLVMValueRef left = llvm_expr_get(llvm, expr->binop.left);
            LLVMValueRef right = llvm_expr_get(llvm, expr->binop.right);
            return llvm_binop(llvm, left, right, expr->binop.kind);
        }
        case AST_EXPR_INTEGER:
        case AST_EXPR_FP:
        case AST_EXPR_CHAR:
        case AST_EXPR_BOOLEAN:
        case AST_EXPR_NULL:
            assert(0, "must be caught by const val");
    }
    assert(0, "falled through");
}

LLVMValueRef llvm_expr_get(LlvmBackend *llvm, AstExpr *expr) {
    return llvm_value_get(llvm, llvm_expr(llvm, expr), expr->sema.value);
}

LLVMValueRef llvm_const(LlvmBackend *llvm, SemaConst *constant) {
    switch (constant->kind) {
        case SEMA_CONST_INT:
            return LLVMConstInt(llvm_type(constant->type), constant->integer, false);
        case SEMA_CONST_FLOAT:
            return LLVMConstReal(llvm_type(constant->type), constant->fp);
        case SEMA_CONST_BOOL:
            return LLVMConstInt(LLVMInt1Type(), constant->boolean, false);
        case SEMA_CONST_ARRAY:
        case SEMA_CONST_STRUCT:
            assert(0, "this const type is NIY");
    }
    assert(0, "falled through");
}

LLVMValueRef llvm_binop(LlvmBackend *llvm, LLVMValueRef left, LLVMValueRef right, AstBinopKind kind) {
    switch (kind) {
        case AST_BINOP_AND: return LLVMBuildAnd(llvm->builder, left, right, "");
        case AST_BINOP_MOD: return LLVMBuildURem(llvm->builder, left, right, "");
        case AST_BINOP_ADD: return LLVMBuildAdd(llvm->builder, left, right, "");
        case AST_BINOP_SUB: return LLVMBuildSub(llvm->builder, left, right, "");
        case AST_BINOP_MUL: return LLVMBuildMul(llvm->builder, left, right, "");
        case AST_BINOP_DIV: return LLVMBuildUDiv(llvm->builder, left, right, "");
        case AST_BINOP_EQ: return LLVMBuildICmp(llvm->builder, LLVMIntEQ, left, right, "");
        case AST_BINOP_NEQ: return LLVMBuildICmp(llvm->builder, LLVMIntNE, left, right, "");
        case AST_BINOP_GT: return LLVMBuildICmp(llvm->builder, LLVMIntUGT, left, right, "");
        case AST_BINOP_GE: return LLVMBuildICmp(llvm->builder, LLVMIntUGE, left, right, "");
        case AST_BINOP_LT: return LLVMBuildICmp(llvm->builder, LLVMIntULT, left, right, "");
        case AST_BINOP_LE: return LLVMBuildICmp(llvm->builder, LLVMIntULE, left, right, "");
        case AST_BINOP_OR: return LLVMBuildOr(llvm->builder, left, right, "");
        case AST_BINOP_BITAND: return LLVMBuildAnd(llvm->builder, left, right, "");
        case AST_BINOP_BITOR: return LLVMBuildOr(llvm->builder, left, right, "");
        case AST_BINOP_SHR: return LLVMBuildLShr(llvm->builder, left, right, "");
        case AST_BINOP_SHL: return LLVMBuildShl(llvm->builder, left, right, "");
        case AST_BINOP_XOR: return LLVMBuildXor(llvm->builder, left, right, "");
    }
    assert(0, "invalid binop");
}
