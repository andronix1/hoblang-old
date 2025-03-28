#pragma once

#include "ast/interface/body.h"
#include "sema/interface/module.h"

void sema_analyze_body(SemaModule *sema, AstBody *body);
