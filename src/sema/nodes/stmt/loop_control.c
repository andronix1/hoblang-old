#include "loop_control.h"
#include "sema/module.h"

void sema_analyze_stmt_loop_control(SemaModule *sema, FileLocation loc, AstLoopControl *control) {
    SemaLoop *loop = sema_module_resolve_loop(sema, control->is_labelled ? &control->label : NULL);
    if (!loop) {
        SEMA_ERROR(loc, "loop control outside of specified loop");
        return;
    }
    control->sema.loop = loop;
}
