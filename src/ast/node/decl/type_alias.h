#pragma once

#include "ast/interface/type.h"
#include "core/location.h"
#include "core/slice/slice.h"

typedef struct {
    Slice name;
    FileLocation name_loc;
    AstType *type;
} AstTypeAlias;

static inline AstTypeAlias ast_type_alias_new(Slice name, FileLocation name_loc, AstType *type) {
    AstTypeAlias result = {
        .name = name,
        .name_loc = name_loc,
        .type = type
    };
    return result;
}
