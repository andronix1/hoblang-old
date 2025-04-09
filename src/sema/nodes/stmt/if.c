#include "if.h"
#include "ast/shared/expr.h"
#include "core/vec.h"
#include "sema/module.h"
#include "sema/nodes/shared/body.h"
#include "sema/nodes/shared/expr.h"
#include "sema/type/type.h"

bool sema_analyze_if_block(SemaModule *sema, AstIfCondBlock *block) {
    SemaType *cond = sema_analyze_runtime_expr(sema, block->cond, sema_expr_ctx_new(sema_type_primitive_bool()));
    if (cond && !sema_types_equals(cond, sema_type_primitive_bool())) {
        SEMA_ERROR(block->cond->loc, "if condition must be boolean");
    }
    return sema_analyze_body(sema, block->body);
}

bool sema_analyze_stmt_if(SemaModule *sema, AstIf *if_else) {
    bool passes = sema_analyze_if_block(sema, if_else->if_block);
    for (size_t i = 0; i < vec_len(if_else->else_ifs); i++) {
        if (sema_analyze_if_block(sema, if_else->else_ifs[i])) {
            passes = true;
        }
    }
    if (if_else->else_body) {
        if (sema_analyze_body(sema, if_else->else_body)) {
            passes = true;
        }
    } else {
        passes = true;
    }
    return passes;
}
