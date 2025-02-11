#include "sema/type/type.h"
#include "sema/module/private.h"
#include "sema/value.h"
#include "ast/path.h"

SemaType *sema_resolve_value_path(SemaModule *sema, AstPath *path);
SemaType *sema_resolve_type_path(SemaModule *sema, AstPath *path);
SemaScopeDecl *sema_resolve_decl_path(SemaModule *sema, AstDeclPath *path);

SemaValue *sema_resolve_inner_value_path(SemaModule *sema, SemaType *type, AstInnerPath *path, size_t segment_idx);
SemaValue *sema_resolve_path(SemaModule *sema, AstPath *path);
