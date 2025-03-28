#pragma once

#include "ast/node/stmt/return.h"
#include "core/location.h"
#include "sema/interface/module.h"

void sema_analyze_stmt_return(SemaModule *sema, FileLocation loc, AstReturn *ret);
