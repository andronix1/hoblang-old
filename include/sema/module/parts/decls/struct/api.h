#pragma once

#include <stdint.h>
#include "core/slice.h"
#include "core/location.h"
#include "ast/api/module_node.h"
#include "sema/module.h"

ssize_t sema_get_struct_member_id(SemaModule *sema, FileLocation at, AstStructDef *struct_def, Slice *name);
