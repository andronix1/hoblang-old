#include "defer.h"
#include "sema/module.h"
#include "sema/nodes/shared/body.h"

void sema_analyze_stmt_defer(SemaModule *sema, FileLocation loc, AstDefer *defer) {
    if (!sema_analyze_body(sema, defer->body)) {
        SEMA_ERROR(loc, "defer body must not be unfinished");
    }
    sema_module_push_defer(sema, defer->body);
}
