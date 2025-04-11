#pragma once

#include <malloc.h>
#include <stdbool.h>
#include "ast/interface/node.h"
#include "sema/interface/body_break.h"

typedef struct AstBody {
    AstNode **nodes;

    struct {
        bool breaks;
        SemaBodyBreak *body_break;
    } sema;
} AstBody;

static inline AstBody *ast_body_new(AstNode **nodes) {
    AstBody *result = malloc(sizeof(AstBody));
    result->nodes = nodes;
    return result;
}
