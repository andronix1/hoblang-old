#pragma once

#include "ast/interface/body.h"
#include "core/vec.h"

typedef struct SemaBodyBreak {
    AstBody **defers;
} SemaBodyBreak;

static inline SemaBodyBreak *sema_body_break() {
    SemaBodyBreak *result = malloc(sizeof(SemaBodyBreak));
    result->defers = vec_new(AstBody*);
    return result;
}
