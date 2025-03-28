#pragma once

#include "sema/interface/value.h"
#include "ast/shared/path.h"

SemaValue *sema_path_runtime_ident(SemaModule *sema, SemaValue *in, AstPathSegment *segment);
