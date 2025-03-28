#pragma once

#include "ast/shared/path.h"
#include "sema/interface/value.h"

SemaValue *sema_resolve_path_module(SemaModule *sema, SemaModule *in, AstPathSegment *segment);

