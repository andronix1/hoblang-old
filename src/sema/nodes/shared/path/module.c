#include "module.h"
#include "core/not_null.h"
#include "sema/module.h"
#include "sema/decl.h"

SemaValue *sema_resolve_path_module(SemaModule *sema, SemaModule *in, AstPathSegment *segment) {
    switch (segment->kind) {
        case AST_PATH_SEGMENT_IDENT: return NOT_NULL(sema_module_resolve_decl(in, &segment->ident, NULL))->value;
        default:
            SEMA_ERROR(segment->loc, "cannot get this path from module");
            return NULL;
    }
}

