#include "exprs.h"

SemaValue *sema_analyze_expr_get_local(SemaModule *sema, AstExprGetLocal *get_local, SemaExprCtx ctx) { 
    return sema_resolve_path(sema, &get_local->path);
}
