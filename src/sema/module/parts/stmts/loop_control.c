#include "ast/private/stmts/loop_control.h"
#include "sema/module/loop/api.h"
#include "sema/module.h"

void sema_stmt_loop_control(SemaModule *sema, AstStmtLoopControl *loop_control) {
    SemaLoop *loop = loop_control->is_named ?
        sema_module_named_loop(sema, &loop_control->loop_name) :
        sema_module_top_loop(sema);
    if (!loop) {
        return;
    }
    loop_control->loop = loop;
}
