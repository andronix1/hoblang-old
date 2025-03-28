#include "ast/private/type.h"
#include "ast/private/expr.h"
#include "sema/arch/bits/private.h"
#include "sema/const/const.h"
#include "sema/module/private.h"
#include "sema/module/parts/expr.h"
#include "sema/type/api.h"
#include "sema/type/private.h"
#include "sema/module/parts/type.h"
#include "sema/module/parts/path.h"
#include "core/vec.h"

SemaType *sema_ast_type(SemaModule *sema, AstType *type) {
	if (type->sema) {
		return type->sema;
	}
	switch (type->type) {
		case AST_TYPE_FUNCTION: {
			SemaType **args = vec_new(SemaType*);
			for (size_t i = 0; i < vec_len(type->func.args); i++) {
				SemaType* arg_type = sema_ast_type(sema, type->func.args[i]);
				if (!arg_type) {
					return NULL;
				}
				args = vec_push(args, &arg_type);
			}
			SemaType *stype = malloc(sizeof(SemaType));
			stype->type = SEMA_TYPE_FUNCTION;
			stype->func.args = args;
			stype->func.returning = sema_ast_type(sema, type->func.returns);
			type->sema = stype;
			break;
		}
		case AST_TYPE_OPTIONAL: {
			SemaType *stype = sema_ast_type(sema, type->optional_of);
            if (!stype) {
                return NULL;
            }
            type->sema = sema_type_new_optional(stype);
			break;
		}
		case AST_TYPE_PATH: {
			type->sema = sema_resolve_type_path(sema, NULL, type->path);
			break;
		}
		case AST_TYPE_ARRAY: {
			SemaType *array_of = sema_ast_type(sema, type->array.of);
			if (!array_of) {
				return NULL;
			}
            SemaType *size_type = sema_arch_usize(sema);
            SemaConst *size = sema_const_expr(sema, type->array.length, sema_expr_ctx_default_of(size_type));
            if (!size) {
                return NULL;
            }
            if (!sema_types_equals(size->sema_type, size_type)) {
                SEMA_ERROR(type->array.length->loc, "array size must be usize, not {sema::type}", size->sema_type);
                return NULL;
            }
            if (size->integer <= 0) {
                SEMA_ERROR(type->array.length->loc, "array size must be positive, not {long}", size->integer);
                return NULL;
            }
			type->sema = sema_type_new_array(size->integer, array_of);
			break;
		}
		case AST_TYPE_SLICE: {
			SemaType *slice_of = sema_ast_type(sema, type->slice_of);
			if (!slice_of) {
				return NULL;
			}
			type->sema = sema_type_new_slice(slice_of);
			break;
		}
		case AST_TYPE_POINTER: {
			SemaType *ptr_to = sema_ast_type(sema, type->ptr_to);
			if (!ptr_to) {
				return NULL;
			}
			type->sema = sema_type_new_pointer(ptr_to);
			break;
		}
	}
	return type->sema;
}
