#pragma once

#include "ast/interface/expr.h"
#include "ast/interface/path.h"
#include "core/slice/slice.h"

typedef struct {
    Slice name;
    FileLocation name_loc;
    AstExpr *value;
} AstExprStructureField;

typedef struct {
    AstPath *path;
    AstExprStructureField *fields;
} AstExprStructure;

static inline AstExprStructureField ast_expr_structure_field(Slice name, FileLocation name_loc, AstExpr *value) {
    AstExprStructureField result = {
        .name = name,
        .name_loc = name_loc,
        .value = value
    };
    return result;
}
