#pragma once

#include "ast/node/stmt/while.h"
#include "sema/interface/module.h"

void sema_analyze_stmt_while(SemaModule *sema, AstWhile *while_loop);
