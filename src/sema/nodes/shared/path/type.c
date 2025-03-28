#include "type.h"
#include "ast/shared/path.h"
#include "sema/arch/info.h"
#include "sema/module.h"
#include "sema/value.h"

SemaValue *sema_resolve_path_type(SemaModule *sema, SemaType *in, AstPathSegment *segment) {
    if (segment->kind == AST_PATH_SEGMENT_SIZEOF) {
        segment->sema.kind = SEMA_PATH_SEG_SIZEOF_TYPE;
        return sema_value_new_runtime(sema_value_runtime_new_final(sema_arch_info_usize(sema_module_arch_info(sema))));
    }
    SEMA_ERROR(segment->loc, "cannot get path from there");
    return NULL;
}
