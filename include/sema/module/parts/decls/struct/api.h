#pragma once

#include "struct.h"
#include "core/slice.h"
#include "core/location.h"
#include "ast/api/module_node.h"
#include "sema/module.h"

SemaStructMember *sema_get_struct_member(SemaModule *sema, FileLocation at, AstStructDef *struct_def, Slice *name);
