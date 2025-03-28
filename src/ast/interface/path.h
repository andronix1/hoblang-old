#pragma once

#include "core/location.h"
#include "core/slice/slice.h"
typedef struct AstPath AstPath;

FileLocation ast_path_loc(AstPath *path);
Slice ast_path_name(AstPath *path);
