#include "path.h"
#include "ast/shared/path.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "sema/module.h"
#include "sema/nodes/shared/path/module.h"
#include "sema/nodes/shared/path/runtime.h"
#include "sema/nodes/shared/path/type.h"
#include "sema/value.h"
#include "core/assert.h"

SemaValue *_sema_resolve_inner_path(SemaModule *sema, SemaValue *value, AstPath *path, size_t start) {
    for (size_t i = start; i < vec_len(path->segments); i++) {
        AstPathSegment *segment = &path->segments[i];
        segment->sema.from_value = value;
        switch (value->kind) {
            case SEMA_VALUE_RUNTIME:
                value = NOT_NULL(sema_resolve_path_runtime(sema, value, segment));
                break;
            case SEMA_VALUE_COMP_TIME:
                switch (value->comp_time.kind) {
                    case SEMA_VALUE_COMPTIME_MODULE:
                        value = NOT_NULL(sema_resolve_path_module(sema, value->comp_time.module, segment));
                        break;
                    case SEMA_VALUE_COMPTIME_TYPE:
                        value = NOT_NULL(sema_resolve_path_type(sema, value->comp_time.type, segment));
                        break;
                    case SEMA_VALUE_COMPTIME_GENERIC_PARAM:
                    case SEMA_VALUE_COMPTIME_GENERIC:
                    case SEMA_VALUE_COMPTIME_BEHAVIOUR:
                        SEMA_ERROR(segment->loc, "cannot get path from here");
                        return NULL;
                }
                break;
        }
    }
    return value;
}

SemaValue *sema_resolve_inner_path(SemaModule *sema, SemaValue *value, AstPath *path) {
    return _sema_resolve_inner_path(sema, value, path, 0);
}

SemaValue *sema_resolve_path(SemaModule *sema, AstPath *path) {
    assert(vec_len(path->segments) > 0, "empty path");
    AstPathSegment *segment = &path->segments[0];
    assert(segment->kind == AST_PATH_SEGMENT_IDENT, "first segment must be an ident");
    segment->sema.kind = SEMA_PATH_SEG_DECL;
    SemaDecl *decl = segment->sema.decl = sema_module_resolve_req_decl(sema, segment->loc, &segment->ident, NULL);
    SemaValue *value = NOT_NULL(decl)->value;
    return _sema_resolve_inner_path(sema, value, path, 1);
}
