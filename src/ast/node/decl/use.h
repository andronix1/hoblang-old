#pragma once

#include "ast/interface/path.h"

typedef struct {
    AstPath *path;
} AstUse;

static inline AstUse ast_use_new(AstPath *path) {
    AstUse result = {
        .path = path 
    };
    return result;
}
