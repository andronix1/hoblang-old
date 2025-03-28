#include "path.h"
#include "ast/shared/path.h"
#include "core/vec.h"

FileLocation ast_path_loc(AstPath *path) { return path->segments[0].loc; }

Slice ast_path_name(AstPath *path) {
    AstPathSegment *top_seg = vec_top(path->segments);
    assert(top_seg->kind == AST_PATH_SEGMENT_IDENT, "oopsie)");
    return top_seg->ident;
}
