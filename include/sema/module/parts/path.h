#include "sema/type/type.h"
#include "sema/module/private.h"
#include "sema/value.h"
#include "ast/api/path.h"

SemaScopeDecl *sema_resolve_decl_path_raw(SemaModule *sema, AstDeclPath *path);
SemaValue *sema_resolve_decl_path(SemaModule *sema, AstDeclPath *path);
SemaValue *sema_resolve_inner_path(SemaModule *sema, SemaValue *from, AstInnerPath *path);
SemaValue *sema_resolve_path(SemaModule *sema, AstPath *path);
SemaType *sema_resolve_type_path(SemaModule *sema, AstPath *path);
