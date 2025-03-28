#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "ast/node/decl/struct.h"
#include "sema/interface/const.h"
#include "sema/interface/type.h"

typedef enum {
    SEMA_CONST_INT,
    SEMA_CONST_FLOAT,
    SEMA_CONST_BOOL,
    SEMA_CONST_ARRAY,
    SEMA_CONST_STRUCT
} SemaConstKind;

typedef struct {
    AstStructDecl *decl;
    SemaConst **fields;
} SemaConstStruct;

typedef struct SemaConst {
    SemaConstKind kind;
    SemaType *type;
    union {
        uint64_t integer;
        long double fp;
        bool boolean;
        SemaConst **array;
        SemaConstStruct structure;
    };
} SemaConst;

static inline SemaConst *sema_const_new_structure(SemaType *type, AstStructDecl *decl, SemaConst **values) {
    SemaConst *result = malloc(sizeof(SemaConst));
    result->kind = SEMA_CONST_STRUCT;
    result->type = type;
    result->structure.decl = decl;
    result->structure.fields = values;
    return result;
}

static inline SemaConst *sema_const_new_array(SemaType *type, SemaConst **values) {
    SemaConst *result = malloc(sizeof(SemaConst));
    result->kind = SEMA_CONST_ARRAY;
    result->type = type;
    result->array= values;
    return result;
}

static inline SemaConst *sema_const_new_bool(SemaType *type, bool boolean) {
    SemaConst *result = malloc(sizeof(SemaConst));
    result->kind = SEMA_CONST_BOOL;
    result->type = type;
    result->boolean = boolean;
    return result;
}

static inline SemaConst *sema_const_new_fp(SemaType *type, long double fp) {
    SemaConst *result = malloc(sizeof(SemaConst));
    result->kind = SEMA_CONST_FLOAT;
    result->type = type;
    result->fp = fp;
    return result;
}

static inline SemaConst *sema_const_new_int(SemaType *type, uint64_t integer) {
    SemaConst *result = malloc(sizeof(SemaConst));
    result->kind = SEMA_CONST_INT;
    result->type = type;
    result->integer = integer;
    return result;
}
