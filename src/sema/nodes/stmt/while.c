#include "while.h"
#include "core/not_null.h"
#include "ast/shared/expr.h"
#include "sema/interface/type.h"
#include "sema/module.h"
#include "sema/nodes/shared/body.h"
#include "sema/nodes/shared/expr.h"
#include "sema/type/type.h"

void sema_analyze_stmt_while(SemaModule *sema, AstWhile *while_loop) {
    SemaType *cond_type = RET_ON_NULL(sema_analyze_runtime_expr(sema, while_loop->cond, sema_expr_ctx_new(sema_type_primitive_bool())));
    if (!sema_types_equals(cond_type, sema_type_primitive_bool())) {
        SEMA_ERROR(while_loop->cond->loc, "while loop condition must be boolean, not {sema::type}", cond_type);
    }
    sema_analyze_body(sema, while_loop->body);
}
