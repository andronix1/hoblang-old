#pragma once

#include "ast/interface/path.h"
#include "sema/interface/behaviour.h"
#include <stdlib.h>

typedef struct AstBehaviour {
    AstPath **paths;

    struct {
        SemaBehaviour *behaviour;
        SemaType *self;
    } sema;
} AstBehaviour;

static inline AstBehaviour *ast_behaviour_new(AstPath **paths) {
    AstBehaviour *result = malloc(sizeof(AstBehaviour));
    result->paths = paths;
    return result;
}
