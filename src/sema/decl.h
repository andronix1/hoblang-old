#pragma once

#include <malloc.h>
#include "core/slice/slice.h"
#include "sema/interface/value.h"
#include "sema/interface/type.h"

typedef struct SemaDecl {
    Slice name;
    SemaType *in_type;
    SemaValue *value;
} SemaDecl;

static inline SemaDecl *sema_decl_new_in_type(Slice name, SemaType *in_type, SemaValue *value) {
    SemaDecl *result = malloc(sizeof(SemaDecl));
    result->name = name;
    result->in_type = in_type;
    result->value = value;
    return result;
}

static inline SemaDecl *sema_decl_new(Slice name, SemaValue *value) {
    return sema_decl_new_in_type(name, NULL, value);
}
