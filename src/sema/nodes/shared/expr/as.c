#include "as.h"
#include "as_kind.h"
#include "ast/shared/expr/as.h"
#include "core/location.h"
#include "core/not_null.h"
#include "sema/arch/info.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/expr/as_kind.h"
#include "sema/value.h"
#include "sema/module.h"
#include "core/assert.h"
#include "sema/nodes/shared/expr.h"
#include "sema/nodes/shared/type.h"
#include "sema/type/type.h"

#define SEMA_AS_NEST sema_runtime_value_nest_with(inner_value, as)
#define SEMA_AS_FINAL sema_value_new_runtime(sema_value_runtime_new_final(as))

SemaValue *_sema_module_analyze_expr_as(SemaModule *sema, FileLocation loc, SemaValue *inner_value, SemaType *as, SemaAsKind *as_kind) {
    SemaType *inner = sema_value_is_runtime(inner_value);
    if (!inner) {
        SEMA_ERROR(loc, "inner as expression must be a runtime value");
        return NULL;
    }
    if (sema_types_equals(inner, as)) {
        *as_kind = SEMA_AS_IGNORE;
        return inner_value;
    }
    switch (inner->kind) {
        case SEMA_TYPE_OPTIONAL: {
            if (!sema_types_equals(inner->optional_of, as)) {
                SEMA_ERROR(loc, "optionals can be casted to inner type only, not {sema::type}", as);
                return NULL;
            }
            *as_kind = SEMA_AS_OPT_UNWRAP;
            return SEMA_AS_NEST;
        }
        case SEMA_TYPE_FUNCTION: {
            if (as->kind != SEMA_TYPE_POINTER) {
                SEMA_ERROR(loc, "functions can be casted to pointers only, not {sema::type}", as); 
                return NULL;
            }
            *as_kind = SEMA_AS_PTRCAST;
            return SEMA_AS_FINAL;
        }
        case SEMA_TYPE_POINTER: {
            if (as->kind == SEMA_TYPE_SLICE) {
                if (inner->pointer_to->kind != SEMA_TYPE_ARRAY) {
                    SEMA_ERROR(loc, "only pointers to array can be casted to slice, not {sema::type}", as); 
                    return NULL;
                }
                if (!sema_types_equals(inner->pointer_to->array.of, as->slice_of)) {
                    SEMA_ERROR(loc, "cannot cast pointer to array of {sema::type} to slice {sema::type}", inner->pointer_to->array.of, as); 
                    return NULL;
                }
                *as_kind = SEMA_AS_ARR_TO_SLICE;
                return SEMA_AS_FINAL;
            }
            SemaType *ptr_int = sema_arch_info_ptr(sema_module_arch_info(sema));
            if (sema_types_equals(inner, ptr_int)) {
                *as_kind = SEMA_AS_PTR_TO_INT;
                return SEMA_AS_FINAL;
            } else if (as->kind == SEMA_TYPE_POINTER) {
                *as_kind = SEMA_AS_PTRCAST;
                return SEMA_AS_FINAL;
            } else {
                SEMA_ERROR(loc, "pointer can be casted to {sema::type} or another pointer, not {sema::type}", ptr_int, as); 
                return NULL;
            }
        }
        case SEMA_TYPE_ARRAY: {
            if (as->kind != SEMA_TYPE_SLICE) {
                SEMA_ERROR(loc, "arrays can be casted to slice only, not {sema::type}", as); 
                return NULL;
            }
            if (!sema_types_equals(inner->array.of, as->slice_of)) {
                SEMA_ERROR(loc, "slice must be of inner type of array({sema::type}), not {sema::type}", inner->array.of, as->slice_of); 
                return NULL;
            }
            *as_kind = SEMA_AS_ARR_TO_SLICE;
            return SEMA_AS_FINAL;
        }
        case SEMA_TYPE_STRUCT: {
            SEMA_ERROR(loc, "structures are not castable, sorry :["); 
            return NULL;
        }
        case SEMA_TYPE_GENERIC: {
            SEMA_ERROR(loc, "what do you mean when trying to convert generic type to {sema::type}?)", as); 
            return NULL;
        }
        case SEMA_TYPE_SLICE: {
            SEMA_ERROR(loc, "slices are not castable"); 
            return NULL;
        }
        case SEMA_TYPE_PRIMITIVE: {
            if (as->kind == SEMA_TYPE_POINTER) { 
                SemaType *ptr_int = sema_arch_info_ptr(sema_module_arch_info(sema));
                if (!sema_types_equals(inner, ptr_int)) {
                    SEMA_ERROR(loc, "only {sema::types} can be casted to pointers, not {sema::type}", ptr_int, inner);
                    return NULL;
                }
                *as_kind = SEMA_AS_INT_TO_PTR;
                return SEMA_AS_FINAL;
            } else if (sema_type_is_int(inner)) {
                if (sema_type_is_int(as)) {
                    *as_kind = SEMA_AS_INT_CAST;
                    return SEMA_AS_FINAL;
                } else if (sema_type_is_float(inner)) {
                    *as_kind = SEMA_AS_INT_TO_FLOAT;
                    return SEMA_AS_FINAL;
                } else {
                    SEMA_ERROR(loc, "integers cannot be casted to {sema::type}", as); 
                    return NULL;
                }
            } else if (sema_type_is_float(inner)) {
                if (sema_type_is_int(as)) {
                    *as_kind = SEMA_AS_FLOAT_TO_INT;
                    return SEMA_AS_FINAL;
                } else if (sema_type_is_float(inner)) {
                    *as_kind = SEMA_AS_FLOAT_CAST;
                    return SEMA_AS_FINAL;
                } else {
                    SEMA_ERROR(loc, "float cannot be casted to {sema::type}", as); 
                    return NULL;
                }
            }
            SEMA_ERROR(loc, "{sema::type} cannot be casted to {sema::type}", inner, as); 
            return NULL;
        }
    }
    assert(0, "illegal type");
}

SemaValue *sema_module_analyze_expr_as(SemaModule *sema, AstExprAs *as, SemaExprCtx ctx) {
    SemaType *as_type = NULL;
    switch (as->kind) {
        case AST_EXPR_AS_AUTO:
            if (!ctx.expectation) {
                SEMA_ERROR(as->as_auto_loc, "as-auto expression must expect something type");
                return NULL;
            }
            as_type = ctx.expectation;
            break;
        case AST_EXPR_AS_TYPE:
            as_type = NOT_NULL(sema_resolve_type(sema, as->type));
            break;
    }
    assert(as_type, "invalid as-expression");
    SemaValue *inner = NOT_NULL(sema_analyze_expr(sema, as->inner, sema_expr_ctx_with(ctx, as_type)));
    return _sema_module_analyze_expr_as(sema, as->as_loc, inner, as_type, &as->sema.kind);
}
