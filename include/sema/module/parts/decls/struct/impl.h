#pragma once

#include <stddef.h>
#include <malloc.h>
#include "sema/module/decls.h"

typedef enum {
    SEMA_STRUCT_MEMBER_FIELD,
    SEMA_STRUCT_MEMBER_EXT_FUNC,
} SemaStructMemberType;

typedef struct SemaStructMember {
    SemaStructMemberType type;

    union {
        size_t field_idx;
        struct {
            bool is_ptr;
            SemaScopeValueDecl *decl;
        } ext_func;
    };
} SemaStructMember;

static inline SemaStructMember *sema_struct_member_field(size_t field_idx) {
    SemaStructMember *result = malloc(sizeof(SemaStructMember));
    result->type = SEMA_STRUCT_MEMBER_FIELD;
    result->field_idx = field_idx;
    return result;
}

static inline SemaStructMember *sema_struct_member_ext_func(SemaScopeValueDecl *ext_func, bool is_ptr) {
    SemaStructMember *result = malloc(sizeof(SemaStructMember));
    result->type = SEMA_STRUCT_MEMBER_EXT_FUNC;
    result->ext_func.decl = ext_func;
    result->ext_func.is_ptr = is_ptr;
    return result;
}
