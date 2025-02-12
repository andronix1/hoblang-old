#pragma once

#include "sema/type/type.h"
#include "sema/module/module.h"
#include "ast/api/type.h"

SemaType *sema_ast_type(SemaModule *sema, AstType *type);
