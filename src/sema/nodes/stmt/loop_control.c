#include "loop_control.h"
#include "sema/interface/body_break.h"
#include "sema/module.h"
#include "sema/loop.h"

void sema_analyze_stmt_loop_control(SemaModule *sema, FileLocation loc, AstLoopControl *control) {
    SemaLoop *loop = sema_module_resolve_loop(sema, control->is_labelled ? &control->label : NULL);
    if (!loop) {
        SEMA_ERROR(loc, "loop control outside of specified loop");
        return;
    }
    control->sema.body_break = sema_module_bb_collect_until(sema, loop->body);
    control->sema.loop = loop;
}
