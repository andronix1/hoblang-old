#pragma once

#include "ast/node/stmt/defer.h"
#include "core/location.h"
#include "sema/interface/module.h"

void sema_analyze_stmt_defer(SemaModule *sema, FileLocation loc, AstDefer *defer);
