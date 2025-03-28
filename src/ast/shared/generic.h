#pragma once

#include <malloc.h>
#include "ast/interface/behaviour.h"
#include "core/location.h"
#include "core/slice/slice.h"

typedef struct AstGenericParam {
    Slice name;
    FileLocation name_loc;
    AstBehaviour *behaviours;
} AstGenericParam;

typedef struct AstGenerics {
    AstGenericParam *params;
} AstGenerics;

static inline AstGenerics* ast_generics_new(AstGenericParam *params) {
    AstGenerics *result = malloc(sizeof(AstGenerics));
    result->params = params;
    return result;
}

static inline AstGenericParam ast_generic_param_new(Slice name, FileLocation name_loc, AstBehaviour *behaviours) {
    AstGenericParam result;
    result.name_loc = name_loc;
    result.name = name;
    result.behaviours = behaviours;
    return result;
}
