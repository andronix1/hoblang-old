#include "type.h"
#include "ast/shared/type.h"
#include "ast/shared/expr.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "sema/arch/info.h"
#include "sema/interface/type.h"
#include "sema/interface/value.h"
#include "sema/module.h"
#include "sema/nodes/shared/expr.h"
#include "sema/nodes/shared/path.h"
#include "sema/type/type.h"
#include "sema/project.h"
#include "sema/const.h"

SemaType *_sema_resolve_type(SemaModule *sema, AstType *type) {
    switch (type->kind) {
        case AST_TYPE_PATH: {
            SemaType *path_type = sema_value_is_type(NOT_NULL(sema_resolve_path(sema, type->path)));
            if (!path_type) {
                SEMA_ERROR(ast_path_loc(type->path), "path is not a type");
            }
            return path_type;
        }
        case AST_TYPE_FUNC: {
            SemaType *returns = NOT_NULL(sema_resolve_type(sema, type->func.returns));
            SemaType **args = vec_new(SemaType*);
            for (size_t i = 0; i < vec_len(type->func.args); i++) {
                args = vec_push_dir(args, NOT_NULL(sema_resolve_type(sema, type->func.args[i])));
            }
            return sema_type_new_func(args, returns);
        }
        case AST_TYPE_SLICE: return sema_type_new_slice(NOT_NULL(sema_resolve_type(sema, type->slice_of)));
        case AST_TYPE_ARRAY: {
            SemaConst *constant = sema_value_is_const(NOT_NULL(sema_analyze_expr(sema, type->array.length, sema_expr_ctx_new(sema_arch_info_usize(sema_module_arch_info(sema))))));
            if (!constant) {
                SEMA_ERROR(type->array.length->loc, "array inner expression must be a const");
                return NULL;
            }
            if (!sema_types_equals(constant->type, sema_arch_info_usize(sema_module_arch_info(sema)))) {
                SEMA_ERROR(type->array.length->loc, "length must be an usize");
                return NULL;
            }
            if (constant->integer <= 0) {
                SEMA_ERROR(type->array.length->loc, "array length must positive");
                return NULL;
            }
            return sema_type_new_array(constant->integer, NOT_NULL(sema_resolve_type(sema, type->array.of)));
        }
        case AST_TYPE_OPTIONAL: return sema_type_new_optional(NOT_NULL(sema_resolve_type(sema, type->slice_of)));
        case AST_TYPE_POINTER: return sema_type_new_pointer(NOT_NULL(sema_resolve_type(sema, type->slice_of)));
    }
    assert(0, "invalid ast type {size}", type->kind);
}

SemaType *sema_resolve_type(SemaModule *sema, AstType *type) {
    if (type->sema.type) {
        return type->sema.type;
    }
    return type->sema.type = _sema_resolve_type(sema, type);
}
