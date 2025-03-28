#include "val.h"
#include "core/not_null.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"
#include "sema/nodes/shared/type.h"
#include "sema/nodes/shared/val_info.h"
#include "sema/value.h"
#include "ast/shared/val_info.h"

void sema_module_read_val_decl(SemaModule *sema, AstValDecl *decl, bool public) {
    SemaType *type = decl->info->type ? RET_ON_NULL(sema_resolve_type(sema, decl->info->type)) : NULL;
    SemaValue *value = decl->initializer ? sema_analyze_expr(sema, decl->initializer, sema_expr_ctx_new(type)) : NULL;
    sema_resolve_val_info(sema, decl->info, value, public);
}
