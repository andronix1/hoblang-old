#include "structure.h"
#include "ast/interface/path.h"
#include "ast/node/decl/struct.h"
#include "ast/shared/type.h"
#include "ast/shared/expr/structure.h"
#include "core/not_null.h"
#include "core/slice/api.h"
#include "core/vec.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"
#include "sema/value.h"
#include "sema/type/type.h"
#include "sema/type/type.h"
#include "sema/nodes/shared/path.h"

SemaValue *sema_module_analyze_expr_structure(SemaModule *sema, AstExprStructure *structure) {
    SemaType *type = sema_value_is_type(NOT_NULL(sema_resolve_path(sema, structure->path)));
    if (!type || type->kind != SEMA_TYPE_STRUCT) {
        SEMA_ERROR(ast_path_loc(structure->path), "this is not a structure");
        return NULL;
    }
    AstStructDecl *struct_decl = type->struct_decl;
    for (size_t i = 0; i < vec_len(struct_decl->fields); i++) {
        AstStructDeclField *field = &struct_decl->fields[i];
        bool found = false;
        for (size_t j = 0; j < vec_len(structure->fields); j++) {
            AstExprStructureField *expr_field = &structure->fields[j];
            if (slice_eq(&field->name, &expr_field->name)) {
                found = true;
                break;
            }
        }
        if (!found) {
            SEMA_ERROR(ast_path_loc(structure->path), "field `{slice}` is missing", &field->name);
        }
    }
    for (size_t i = 0; i < vec_len(structure->fields); i++) {
        AstExprStructureField *expr_field = &structure->fields[i];
        bool found = false;
        for (size_t j = 0; j < vec_len(struct_decl->fields); j++) {
            AstStructDeclField *field = &struct_decl->fields[j];
            if (slice_eq(&field->name, &expr_field->name)) {
                SemaType *field_type = sema_value_is_type(NOT_NULL(sema_analyze_expr(sema, expr_field->value, sema_expr_ctx_new(field->type->sema.type))));
                if (!field_type) {
                    SEMA_ERROR(expr_field->name_loc, "field `{slice}` expects to have runtime value as initializer", &expr_field->name);
                } else if (!sema_types_equals(field_type, field->type->sema.type)) {
                    SEMA_ERROR(expr_field->name_loc, "field `{slice}` expects to have {sema::type} value as initializer, not {sema::type}", &expr_field->name, field_type, field->type->sema.type);
                }
                found = true;
                break;
            }
        }
        if (!found) {
            sema_analyze_expr(sema, expr_field->value, sema_expr_ctx_new(NULL));
            SEMA_ERROR(expr_field->name_loc, "field `{slice}` isn't present in struct decl", &expr_field->name);
        }
        for (size_t j = 0; j < vec_len(structure->fields); j++) {
            AstExprStructureField *other_field = &structure->fields[j];
            if (slice_eq(&expr_field->name, &other_field->name)) {
                SEMA_ERROR(other_field->name_loc, "field `{slice}` is duplicated", &expr_field->name);
                break;
            }
        }
    }
    return sema_value_new_runtime(sema_value_runtime_new_final(type));
}
