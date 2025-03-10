#pragma once

#include <stdbool.h>
#include "ast/api/val_decl.h"
#include "sema/module.h"
#include "sema/module/decls/decls.h"

SemaDecl *sema_val_decl(SemaModule *sema, AstValDecl *val_decl, bool global);

