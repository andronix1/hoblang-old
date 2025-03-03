#include "sema/type/type.h"
#include "sema/module/private.h"
#include "sema/value.h"
#include "ast/api/path.h"

SemaDecl *sema_resolve_decl_path(SemaModule *sema, AstPath *path);
SemaValue *sema_resolve_path(SemaModule *sema, SemaValue *value, AstPath *path);
SemaType *sema_resolve_type_path(SemaModule *sema, SemaValue *value, AstPath *path);
