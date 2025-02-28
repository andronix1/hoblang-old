#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ast/api/module_node.h"
#include "sema/type.h"

typedef enum {
    SEMA_CONST_INT,
    SEMA_CONST_FLOAT,
    SEMA_CONST_BOOL,
    SEMA_CONST_OPTIONAL,
    SEMA_CONST_STRUCT,
    SEMA_CONST_ARRAY,
} SemaConstType;

typedef struct SemaConst SemaConst;

typedef SemaConst* SemaConstArray;

typedef struct {
    SemaConst *value;
    bool is_null;
} SemaConstOptional;

typedef struct {
    SemaConst *fields;
    AstStructDef *struct_def;
} SemaConstStruct;

typedef struct SemaConst {
    SemaConstType type;

    SemaType *sema_type;

    union {
        int64_t integer;
        long double fp;
        bool boolean;
        SemaConstStruct structure;
        SemaConstOptional optional;
        SemaConstArray array;
    };
} SemaConst;

