#include "ident.h"
#include "core/vec.h"
#include "sema/type/type.h"
#include "sema/nodes/shared/type.h"
#include "sema/value.h"
#include "sema/module.h"
#include "core/slice/api.h"

SemaValue *sema_path_runtime_ident(SemaModule *sema, SemaValue *in, AstPathSegment *segment) {
    SemaType *type = in->runtime.type;
    if (type->kind == SEMA_TYPE_STRUCT) {
        AstStructDecl *decl = type->struct_decl;
        for (size_t i = 0; i < vec_len(decl->fields); i++) {
            if (slice_eq(&decl->fields[i].name, &segment->ident)) {
                segment->sema.kind = SEMA_PATH_SEG_STRUCT_FIELD;
                segment->sema.struct_idx = i;
                return sema_value_new_runtime(sema_value_runtime_new_final(sema_resolve_type(sema, decl->fields[i].type)));
            }
        }
    }

    return NULL;
}

