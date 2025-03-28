#pragma once

#include "ast/shared/path.h"
#include "sema/interface/value.h"
#include "sema/value.h"

SemaValue *sema_resolve_path_type(SemaModule *sema, SemaType *in, AstPathSegment *segment);


