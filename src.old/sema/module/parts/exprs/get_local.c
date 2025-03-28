#include "exprs.h"
#include "sema/module/parts/path.h"

SemaValue *sema_analyze_expr_get_local(SemaModule *sema, AstExprGetLocal *get_local, SemaExprCtx ctx) { 
    return sema_resolve_path(sema, NULL, get_local->path);
}
