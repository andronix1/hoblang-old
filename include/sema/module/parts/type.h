#pragma once

#include "sema/type.h"
#include "sema/module.h"
#include "ast/api/type.h"

SemaType *sema_ast_type(SemaModule *sema, AstType *type);
