#pragma once

#include "core/location.h"
#include "core/slice/slice.h"

typedef struct {
    Slice file_path;
    Slice alias;
    FileLocation file_path_loc, alias_loc;
} AstImport;

static inline AstImport ast_import_new(Slice file_path, FileLocation file_path_loc, Slice alias, FileLocation alias_loc) {
    AstImport result = {
        .file_path = file_path,
        .file_path_loc = file_path_loc,
        .alias = alias,
        .alias_loc = alias_loc,
    };
    return result;
}
