#pragma once

#include "ast/api/expr.h"
#include "ast/private/path.h"
#include "core/location.h"
#include "core/slice.h"

typedef struct {
    FileLocation loc;
    Slice name;
    AstExpr *expr;
    bool is_undefined;

    size_t idx;
} AstExprStructMember;

typedef struct {
    AstPath *path;
    AstExprStructMember *members;

    SemaType *struct_type;
} AstExprStruct;
