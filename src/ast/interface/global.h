#pragma once

#include "core/location.h"
#include "core/slice/slice.h"

typedef struct AstGlobalDeclInfo AstGlobalDeclInfo;

Slice ast_global_decl_info_name(AstGlobalDeclInfo *info);
FileLocation ast_global_decl_info_decl_loc(AstGlobalDeclInfo *info);
