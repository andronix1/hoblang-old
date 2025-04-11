#pragma once

#include "ast/node/stmt/loop_control.h"
#include "core/location.h"
#include "sema/interface/module.h"

void sema_analyze_stmt_loop_control(SemaModule *sema, FileLocation loc, AstLoopControl *control);
