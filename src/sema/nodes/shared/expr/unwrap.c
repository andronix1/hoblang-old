#include "unwrap.h"
#include "core/not_null.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/type/type.h"
#include "ast/shared/expr.h"
#include "sema/nodes/shared/expr.h"
#include "sema/value.h"

SemaValue *sema_module_analyze_expr_unwrap(SemaModule *sema, AstExprUnwrap *unwrap) {
    SemaType *type = sema_value_is_runtime(NOT_NULL(sema_analyze_expr(sema, unwrap->inner, sema_expr_ctx_new(NULL))));
    if (!type) {
        SEMA_ERROR(unwrap->inner->loc, "unwrap value must be runtime");
        return NULL;
    }
    if (type->kind != SEMA_TYPE_OPTIONAL) {
        SEMA_ERROR(unwrap->inner->loc, "only optionals can be unwrapped, not {sema::type}", type);
        return NULL;
    }
    sema_module_push_decl(sema, unwrap->output_name_loc, sema_decl_new(unwrap->output_name, sema_value_new_runtime(sema_value_runtime_new_var(type->optional_of))), false);
    return sema_value_new_runtime(sema_value_runtime_new_final(sema_type_primitive_bool()));
}
