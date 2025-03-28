#pragma once

#include "ast/interface/val_info.h"
#include "sema/interface/module.h"
#include "sema/interface/value.h"

SemaValue *sema_resolve_val_info(SemaModule *sema, AstValInfo *val_info, SemaValue *initializer, bool public);
