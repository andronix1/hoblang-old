#include "exprs.h"
#include "ast/private/module_node.h"
#include "sema/module/parts/expr.h"
#include "sema/type/api.h"
#include "sema/type/private.h"
#include "sema/module/parts/path.h"
#include "core/vec.h"
#include "sema/value/private.h"

SemaValue *sema_analyze_expr_struct(SemaModule *sema, FileLocation at, AstExprStruct *structure, SemaExprCtx ctx) {
    SemaType *type = sema_resolve_type_path(sema, NULL, &structure->path);
    if (!type) {
        return NULL;
    }
    structure->struct_type = type;
    if (type->type != SEMA_TYPE_STRUCT) {
        SEMA_ERROR(structure->path.segments[0].loc, "{ast::path} is not a structure", &structure->path);
        return NULL;
    }
    if (type->struct_def->module != sema) {
        SEMA_ERROR(structure->path.segments[0].loc, "struct constructors are always private, so `{ast::path}` may be called inside a module it's declared", &structure->path);
        return NULL;
    }
    for (size_t i = 0; i < vec_len(structure->members); i++) {
        AstExprStructMember *member1 = &structure->members[i];
        for (size_t j = i + 1; j < vec_len(structure->members); j++) {
            AstExprStructMember *member2 = &structure->members[j];
            if (slice_eq(&member1->name, &member2->name)) {
                SEMA_ERROR(member2->loc, "duplicates field `{slice}` in struct constructor", &member1->name);
            }
        }
    }
    AstStructDef *struct_def = type->struct_def;
    if (vec_len(structure->members) < vec_len(struct_def->members)) {
        SEMA_ERROR(at, "missing fields in struct constructor");
    }
    for (size_t i = 0; i < vec_len(structure->members); i++) {
        AstExprStructMember *member = &structure->members[i];
        bool found = false;
        if (member->is_undefined) {
            continue;
        }
        for (size_t j = 0; j < vec_len(struct_def->members); j++) {
            AstStructMember *struct_member = &struct_def->members[j];
            if (!slice_eq(&struct_member->name, &member->name)) {
                continue;
            }
            found = true;
            member->idx = j;
            SemaType *member_type = sema_value_expr_type(sema, member->expr, sema_expr_ctx_expect(ctx, struct_member->type->sema));
            if (!sema_types_equals(member_type, struct_member->type->sema)) {
                SEMA_ERROR(member->loc, "invalid struct member `{slice}` type. Expected {sema::type}, got {sema::type}", &member->name, struct_member->type->sema, member_type);
            }
            break;
        }
        if (!found) {
            SEMA_ERROR(member->loc, "there is no field `{slice}` in struct `{ast::path}`", &member->name, &structure->path);
        }
    }
    return sema_value_final(type);
}
