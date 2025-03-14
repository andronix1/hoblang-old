#include "sema/module/parts/val_decl.h"
#include "ast/private/val_decl.h"
#include "ast/private/expr.h"
#include "sema/module/decls/api.h"
#include "sema/module/parts/expr.h"
#include "sema/module/parts/type.h"
#include "sema/module/private.h"
#include "sema/type/api.h"
#include "sema/value/api.h"
#include "sema/value/private.h"
#include "core/assert.h"

SemaDecl *sema_val_decl(SemaModule *sema, AstValDecl *val_decl, bool is_global) {
    SemaType *expected = NULL;
    if (val_decl->spec_type) {
        expected = sema_ast_type(sema, val_decl->spec_type);
    }
    SemaValue *value = NULL;
    if (val_decl->initializer) {
        value = sema_expr(sema, val_decl->initializer, sema_expr_ctx_default_of(expected));
        if (!value) {
            return NULL;
        }
        if (!sema_value_is_value(value)) {
            SEMA_ERROR(val_decl->loc, "only values can be assigned to value declaration `{slice}`");
            return NULL;
        }
        if (is_global && !sema_value_is_not_mut(value)) {
            SEMA_ERROR(val_decl->loc, "global declaration's initializer must be constant or final");
            return NULL;
        }
        SemaType *expr_type = sema_value_typeof(value);
        if (expected && !sema_types_equals(expr_type, expected)) {
            SEMA_ERROR(val_decl->initializer->loc, "specified type {sema::type} but initializer of type {sema::type}", expected, expr_type);
            return NULL;
        }
        expected = expr_type;
    }
    if (!expected) {
        SEMA_ERROR(val_decl->loc, "failed to resolve declaration path. Type or initializer must be specified");
        return NULL;
    }
    switch (val_decl->type) {
        case AST_VAL_DECL_CONST:
            if (!value) {
                SEMA_ERROR(val_decl->loc, "const declaration must be initialized");
                return NULL;
            }
            if (!is_global && !sema_value_is_const(value)) {
                SEMA_ERROR(val_decl->loc, "cannot assign non-const values to final");
                return NULL;
            }
            return val_decl->sema.decl = sema_decl_new(val_decl->name, value);
        case AST_VAL_DECL_FINAL:
            return val_decl->sema.decl = sema_decl_new(val_decl->name, sema_value_final(expected));
        case AST_VAL_DECL_VAR:
            return val_decl->sema.decl = sema_decl_new(val_decl->name, sema_value_var(expected));
    }
    assert(0, "falled through");
}
