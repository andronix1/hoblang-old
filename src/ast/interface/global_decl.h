#pragma once

#include "core/location.h"
#include "core/slice/slice.h"

typedef struct AstGlobalDecl AstGlobalDecl;

Slice ast_global_decl_name(AstGlobalDecl *info);
FileLocation ast_global_decl_decl_loc(AstGlobalDecl *info);
