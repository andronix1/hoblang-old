#pragma once

#include "ast/api/generic.h"
#include "sema/type.h"
#include "sema/module.h"

SemaType *sema_analyze_generic_param(SemaModule *sema, AstGenericParam *param);
