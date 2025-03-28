#include "return.h"
#include "core/location.h"
#include "core/not_null.h"
#include "ast/shared/expr.h"
#include "sema/interface/type.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"
#include "sema/type/type.h"

void sema_analyze_stmt_return(SemaModule *sema, FileLocation loc, AstReturn *ret) {
    SemaType *returns = RET_ON_NULL(sema_module_returns(sema));
    if (!ret->value) {
        if (!sema_types_equals(returns, sema_type_primitive_void())) {
            SEMA_ERROR(loc, "no return value in function, that returns {sema::type}", returns);
        }
    } else {
        SemaType *type = RET_ON_NULL(sema_analyze_runtime_expr(sema, ret->value, sema_expr_ctx_new(returns)));
        if (!sema_types_equals(returns, type)) {
            SEMA_ERROR(ret->value->loc, "expected to return {sema::type} but got {sema::type}", returns, type);
        }
    }
}
