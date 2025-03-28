#pragma once

#include "ast/interface/body.h"
#include <stdlib.h>

typedef struct {
    AstBody *body;
} AstDefer;

static inline AstDefer *ast_defer_new(AstBody *body) {
    AstDefer *result = malloc(sizeof(AstDefer));
    result->body = body;
    return result;
}
