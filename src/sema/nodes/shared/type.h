#pragma once

#include "sema/interface/module.h"
#include "sema/interface/type.h"
#include "ast/interface/type.h"

SemaType *sema_resolve_type(SemaModule *sema, AstType *type);
