#pragma once

#include "ast/private/path.h"
#include "core/location.h"
#include "core/slice/slice.h"

typedef struct {
    AstPath **behaviours;
} AstGenericRules;

typedef struct AstGenericParam {
    AstGenericRules rules;
    Slice name;
    FileLocation loc;
} AstGenericParam;

typedef struct AstGeneric {
    AstGenericParam *params;
} AstGeneric;
