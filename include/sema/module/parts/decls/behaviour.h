#pragma once

#include <stdint.h>
#include "ast/private/decls/behaviour.h"
#include "sema/module/behaviour/impl.h"

SemaBehaviour *sema_analyze_behaviour_decl(SemaModule *sema, AstDeclBehaviour *decl);
