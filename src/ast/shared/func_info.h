#pragma once

#include "ast/interface/type.h"
#include "core/location.h"
#include "core/slice/slice.h"
#include <stdlib.h>

typedef struct {
    Slice name;
    FileLocation name_loc;
    AstType *type;
} AstFuncInfoArg;

typedef struct AstFuncInfo {
    Slice name;
    FileLocation name_loc;
    AstFuncInfoArg *args;
    AstType *ext_of;
    AstType *returns;
} AstFuncInfo;

static inline AstFuncInfoArg ast_func_info_arg_new(Slice name, FileLocation name_loc, AstType *type) {
    AstFuncInfoArg result = {
        .name = name,
        .name_loc = name_loc,
        .type = type
    };
    return result;
}

static inline AstFuncInfo *ast_func_info_new(Slice name, FileLocation name_loc, AstFuncInfoArg *args, AstType *ext_of, AstType *returns) {
    AstFuncInfo *result = malloc(sizeof(AstFuncInfo));
    result->args = args;
    result->name = name;
    result->name_loc = name_loc;
    result->returns = returns;
    result->ext_of = ext_of;
    return result;
}
