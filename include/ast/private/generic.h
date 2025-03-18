#pragma once

#include "ast/api/path.h"
#include "core/location.h"
#include "core/slice/slice.h"
#include "sema/module/behaviour/table/api.h"
#include "sema/type/type.h"

typedef struct {
    AstPath **behaviours;
} AstGenericRules;

typedef struct AstGenericParam {
    AstGenericRules rules;
    Slice name;
    FileLocation loc;

    struct {
        SemaType *type;
    } sema;
} AstGenericParam;

typedef struct AstGeneric {
    AstGenericParam *params;

    struct {
        SemaBehaviourTable **tables;
    } sema;
} AstGeneric;
