#include "sema/module/decls/api.h"
#include "sema/module/decls/impl.h"
#include <malloc.h>

SemaDecl *sema_decl_new_in_type(Slice name, SemaType *in_type, SemaValue *value) {
    SemaDecl *result = malloc(sizeof(SemaDecl));
    result->in_type = in_type;
    result->name = name;
    result->value = value;
    return result;
}

SemaDecl *sema_decl_new(Slice name, SemaValue *value) {
    return sema_decl_new_in_type(name, NULL, value);
}

