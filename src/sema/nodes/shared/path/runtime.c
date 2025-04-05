#include "runtime.h"
#include "core/not_null.h"
#include "sema/decl.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/path/runtime/ident.h"
#include "sema/value.h"
#include "sema/type/type.h"
#include "core/assert.h"

SemaValue *sema_resolve_path_value(SemaModule *sema, SemaValue *in_value, AstPathSegment *segment) {
    SemaType *in = in_value->runtime.type;
    switch (segment->kind) {
        case AST_PATH_SEGMENT_IDENT: {
            RET_IF_OK(sema_path_runtime_ident(sema, in_value, segment));
            SemaDecl *decl = sema_module_resolve_decl(sema, &segment->ident, in);
            if (!decl) {
                SEMA_ERROR(segment->loc, "no `{slice}` in {sema::type}", &segment->ident, in);
                return NULL;
            }
            SemaType *type = sema_value_is_runtime(decl->value);
            if (!type) {
                SEMA_ERROR(segment->loc, "ext function `{slice}` can be called on runtime values only", &segment->ident);
                return NULL; 
            }
            segment->sema.kind = SEMA_PATH_SEG_EXT_FUNC_HANDLE;
            segment->sema.ext_func_decl = decl;
            return sema_value_new_runtime_ext_func_handle(type);
        }
        case AST_PATH_SEGMENT_DEREF: {
            if (in->kind != SEMA_TYPE_POINTER) {
                SEMA_ERROR(segment->loc, "trying dereference non-pointer type {sema::type}", in);
                return NULL;
            }
            return sema_value_new_runtime(sema_value_runtime_new_var(in->pointer_to));
        }
        case AST_PATH_SEGMENT_NULL:
        case AST_PATH_SEGMENT_SIZEOF:
        case AST_PATH_SEGMENT_GENERIC:
            SEMA_ERROR(segment->loc, "cannot get this path from here");
            return NULL;
    }
    assert(0, "invalid ast path segment");
}

SemaValue *sema_resolve_path_runtime(SemaModule *sema, SemaValue *in, AstPathSegment *segment) {
    switch (in->runtime.kind) {
        case SEMA_VALUE_RUNTIME_FINAL:
        case SEMA_VALUE_RUNTIME_VAR:
        case SEMA_VALUE_RUNTIME_CONST:
            return sema_resolve_path_value(sema, in, segment);
        case SEMA_VALUE_RUNTIME_EXT_FUNC_HANDLE:
            SEMA_ERROR(segment->loc, "cannot get path from ext func handle");
            return NULL;
    }
    assert(0, "invalid runtime value");
}

