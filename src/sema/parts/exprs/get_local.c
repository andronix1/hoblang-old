#include "../../parts.h"

bool sema_analyze_expr_get_local(SemaModule *sema, AstExprGetLocal *get_local, SemaType *expectation, SemaValue *value) { 
    return sema_resolve_path(sema, &get_local->path, value);
}
