#include "idx.h"
#include "core/not_null.h"
#include "ast/shared/expr.h"
#include "sema/arch/info.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/nodes/shared/expr.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "sema/value.h"
#include <stddef.h>

SemaValue *sema_module_analyze_expr_idx(SemaModule *sema, AstExprIdx *idx) {
    SemaValue *inner_value = NOT_NULL(sema_analyze_expr(sema, idx->inner, sema_expr_ctx_new(NULL)));
    SemaType *type = sema_value_is_runtime(inner_value);
    if (!type) {
        SEMA_ERROR(idx->inner->loc, "inner value must be runtime");
        return NULL;
    }

    SemaType *usize = sema_arch_info_usize(sema_module_arch_info(sema));
    SemaType *idx_type = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, idx->idx, sema_expr_ctx_new(usize))));
    if (!idx_type) {
        SEMA_ERROR(idx->idx->loc, "idx value must be runtime");
        return NULL;
    }

    if (!sema_types_equals(idx_type, usize)) {
        SEMA_ERROR(idx->idx->loc, "idx must be usize");
        return NULL;
    }
    if (type->kind == SEMA_TYPE_ARRAY) {
        // TODO: warning on constants
        idx->sema.kind = SEMA_IDX_ARRAY;
        return sema_runtime_value_nest_with(inner_value, type->array.of);
    } else if (type->kind == SEMA_TYPE_SLICE) {
        idx->sema.kind = SEMA_IDX_SLICE;
        return sema_value_new_runtime(sema_value_runtime_new_var(type->slice_of));
    } else if (type->kind == SEMA_TYPE_POINTER) {
        idx->sema.kind = SEMA_IDX_POINTER;
        return sema_value_new_runtime(sema_value_runtime_new_var(type->slice_of));
    } else {
        SEMA_ERROR(idx->inner->loc, "only arrays or slicis can be indexed");
        return NULL;
    }
}
