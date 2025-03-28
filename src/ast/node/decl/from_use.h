#pragma once

#include <stdbool.h>
#include "ast/interface/path.h"
#include "core/location.h"

typedef enum {
    AST_FROM_USE_ALL,
    AST_FROM_USE_LIST,
} AstFromUseKind;

typedef struct {
    AstFromUseKind kind;
    AstPath *from;
    union {
        AstPath **list;
        FileLocation all_loc;
    };
} AstFromUse;

static inline AstFromUse ast_from_use_new_all(AstPath *from, FileLocation loc) {
    AstFromUse result = {
        .kind = AST_FROM_USE_ALL,
        .from = from,
        .all_loc = loc
    };
    return result;
}

static inline AstFromUse ast_from_use_new_list(AstPath *from, AstPath **list) {
    AstFromUse result = {
        .kind = AST_FROM_USE_LIST,
        .from = from,
        .list = list
    };
    return result;
}
