#pragma once

#include <stddef.h>
#include "sema/module/decls.h"

typedef enum {
    SEMA_STRUCT_MEMBER_FIELD,
    SEMA_STRUCT_MEMBER_EXT_FUNC,
} SemaStructMemberType;

typedef struct SemaStructMember {
    SemaStructMemberType type;

    union {
        size_t field_idx;
        SemaScopeValueDecl *ext_func;
    };
} SemaStructMember;

static inline SemaStructMember *sema_struct_member_field(size_t field_idx) {
    SemaStructMember *result = malloc(sizeof(SemaStructMember));
    result->type = SEMA_STRUCT_MEMBER_FIELD;
    result->field_idx = field_idx;
    return result;
}

static inline SemaStructMember *sema_struct_member_ext_func(SemaScopeValueDecl *ext_func) {
    SemaStructMember *result = malloc(sizeof(SemaStructMember));
    result->type = SEMA_STRUCT_MEMBER_EXT_FUNC;
    result->ext_func = ext_func;
    return result;
}