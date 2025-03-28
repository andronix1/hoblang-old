#pragma once

#include "core/location.h"
#include "core/slice/slice.h"
#include "ast/interface/type.h"
#include <stdbool.h>

typedef struct {
    bool public;
    Slice name;
    FileLocation name_loc;
    AstType *type;
} AstStructDeclField;

typedef struct {
    Slice name;
    FileLocation name_loc;
    AstStructDeclField *fields;
} AstStructDecl;

static inline AstStructDeclField ast_struct_decl_field_new(bool public, Slice name, FileLocation name_loc, AstType *type) {
    AstStructDeclField result = {
        .public = public,
        .name = name,
        .name_loc = name_loc,
        .type = type 
    };
    return result;
}

static inline AstStructDecl ast_struct_decl_new(Slice name, FileLocation name_loc, AstStructDeclField *fields) {
    AstStructDecl result = {
        .name = name,
        .name_loc = name_loc,
        .fields = fields
    };
    return result;
}
