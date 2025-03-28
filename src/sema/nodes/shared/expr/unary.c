#include "core/not_null.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"
#include "sema/value.h"
#include "sema/type/type.h"
#include "ast/shared/expr/unary.h"
#include "ast/shared/expr.h"
#include "core/assert.h"

SemaValue *sema_module_analyze_expr_unary(SemaModule *sema, AstExprUnary *unary, SemaExprCtx ctx) {
    switch (unary->kind) {
        case AST_EXPR_UNARY_REF: {
            SemaType *type = sema_value_is_var(NOT_NULL(sema_analyze_expr(sema, unary->inner,
                    sema_expr_ctx_with(ctx,
                        ctx.expectation && ctx.expectation->kind == SEMA_TYPE_POINTER ?
                            ctx.expectation->pointer_to : NULL))));
            if (!type) {
                SEMA_ERROR(unary->inner->loc, "expression is not a variable so reference cannot be taken");
                return NULL;
            }
            return sema_value_new_runtime(sema_value_runtime_new_final(sema_type_new_pointer(type)));
        }
        case AST_EXPR_UNARY_BOOL: {
            SemaType *type = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, unary->inner, ctx)));
            if (!type) {
                SEMA_ERROR(unary->inner->loc, "unary operator can be allied to runtime values only", type);
                return NULL;
            }
            if (!sema_types_equals(type, sema_type_primitive_bool())) {
                SEMA_ERROR(unary->inner->loc, "boolean unary opeartor can be applied to booleans only, not {sema::type}", type);
                return NULL;
            }
            return sema_value_new_runtime(sema_value_runtime_new_final(type));

        }
        case AST_EXPR_UNARY_ARITHMETIC: {
            SemaType *type = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, unary->inner, ctx)));
            if (!type) {
                SEMA_ERROR(unary->inner->loc, "unary operator can be allied to runtime values only", type);
                return NULL;
            }
            if (!sema_type_is_int(type) && !sema_type_is_float(type)) {
                SEMA_ERROR(unary->inner->loc, "arithmetic opeartors can be applied to intergers ot floats only, not {sema::type}", type);
                return NULL;
            }
            return sema_value_new_runtime(sema_value_runtime_new_final(type));
        }
    }
    assert(0, "invalid unary type");
}
