#include "assign.h"
#include "core/not_null.h"
#include "ast/shared/expr.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"

void sema_analyze_stmt_assign(SemaModule *sema, AstAssign *assign) {
    SemaType *source = sema_value_is_var(RET_ON_NULL(sema_analyze_expr(sema, assign->assign_to, sema_expr_ctx_new(NULL))));
    if (!source) {
        SEMA_ERROR(assign->assign_to->loc, "left side of assign statement must be variable");
        return;
    }
    SemaType *assigned = RET_ON_NULL(sema_analyze_runtime_expr(sema, assign->assign_to, sema_expr_ctx_new(source)));
    if (!sema_types_equals(source, assigned)) {
        SEMA_ERROR(assign->assign_to->loc, "left and right part of assign expression must have one type, but {sema::type} != {sema::type}", source, assigned);
        return; 
    }
    switch (assign->kind) {
        case AST_STMT_ASSIGN_DIRECT: break;
        case AST_STMT_ASSIGN_BINOP:
            if (!sema_type_is_int(assigned) && !sema_type_is_float(assigned)) {
                SEMA_ERROR(assign->assign_to->loc, "cannot apply binop {ast::binop} for {sema::type} and {sema::type}", source, assigned);                
            }
            break;
    }
}

