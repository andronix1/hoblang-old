#pragma once

#include "decls.h"
#include "core/slice.h"
#include "sema/type.h"
#include "sema/value.h"

SemaDecl *sema_decl_new_in_type(Slice name, SemaType *in_type, SemaValue *value);
SemaDecl *sema_decl_new(Slice name, SemaValue *value);
