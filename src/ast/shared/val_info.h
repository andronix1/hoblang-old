#pragma once

#include "ast/interface/type.h"
#include "core/location.h"
#include "core/slice/slice.h"
#include <stdlib.h>

typedef enum {
    AST_VAL_INFO_CONST,
    AST_VAL_INFO_FINAL,
    AST_VAL_INFO_VAR,
} AstValInfoKind;

typedef struct AstValInfo {
    AstValInfoKind kind;
    Slice name;
    FileLocation name_loc;
    AstType *type;
} AstValInfo;

static inline AstValInfo *ast_val_info_new(AstValInfoKind kind, Slice name, FileLocation name_loc, AstType *type) {
    AstValInfo *result = malloc(sizeof(AstValInfo));
    result->kind = kind;
    result->type = type;
    result->name = name;
    result->name_loc = name_loc;
    return result;
}

