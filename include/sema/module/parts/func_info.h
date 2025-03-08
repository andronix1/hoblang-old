#pragma once

#include "ast/private/func_info.h"
#include "ast/private/type.h"
#include "sema/module/impl.h"
#include "sema/type/private.h"

SemaType *sema_ast_func(SemaModule *sema, FileLocation at, SemaType *ext_of, AstFuncArg *args, AstType *returning);
