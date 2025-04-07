#pragma once

#include "ast/interface/path.h"
#include "sema/interface/decl.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"

SemaValue *sema_resolve_inner_path(SemaModule *sema, SemaValue *value, AstPath *path);
SemaValue *sema_resolve_path(SemaModule *sema, AstPath *path);
SemaDecl *sema_resolve_decl_path(SemaModule *sema, SemaModule *in, AstPath *path);
