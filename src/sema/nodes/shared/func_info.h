#pragma once

#include "ast/interface/func_info.h"
#include "sema/interface/module.h"
#include "sema/interface/type.h"

SemaType *sema_resolve_func_info_type(SemaModule *sema, AstFuncInfo *func_info);
void sema_read_func_info(SemaModule *sema, AstFuncInfo *func_info, bool public);
