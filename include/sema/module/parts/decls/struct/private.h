#pragma once

#include "core/slice.h"
#include "sema/module/decls.h"

typedef struct {
    Slice name;
    SemaScopeValueDecl *decl;
} SemaStructExtFunc;