#include "ast/private/path.h"
#include "ast/private/type.h"
#include "ast/private/expr.h"
#include "ast/private/module_node.h"
#include "core/vec.h"
#include "sema/arch/bits/private.h"
#include "sema/const/const.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/module.h"
#include "sema/module/parts/expr.h"
#include "sema/type/private.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "sema/module/parts/decls/struct/api.h"
#include "sema/module/behaviour/api.h"
#include "sema/value/private.h"
#include "sema/value/api.h"
#include "sema/type/private.h"

SemaValue *sema_resolve_inner_value_path(SemaModule *sema, SemaValue *from, AstPathSegment *segment) {
    segment->sema.value = from;
	switch (segment->type) {
        case AST_PATH_SEG_GENERIC: {
            SEMA_ERROR(segment->loc, "{sema::type} is not a generic", sema_value_typeof(from));
            return NULL;
        }
        case AST_PATH_SEG_NULL: {
            if (sema_value_typeof(from)->type != SEMA_TYPE_OPTIONAL) {
                SEMA_ERROR(segment->loc, "cannot determine is value null for type {sema::type}", sema_value_typeof(from));
                return NULL;
            }
            segment->sema.type = SEMA_PATH_IS_NULL;
            segment->sema.optional_type = sema_value_typeof(from);
            return sema_value_with_type(from, sema_type_primitive_bool());
        }
        case AST_PATH_SEG_SIZEOF: {
            segment->sema.type = SEMA_PATH_SIZEOF;
            segment->sema.sizeof_op.type = sema_value_typeof(from);
            segment->sema.sizeof_op.output_type = sema_arch_usize(sema);
            // TODO: sizeof is constant
            return sema_value_final(segment->sema.sizeof_op.output_type);
        }
        case AST_PATH_SEG_DEREF: {
            if (sema_value_typeof(from)->type != SEMA_TYPE_POINTER) {
                SEMA_ERROR(segment->loc, "only pointers can be dereferenced, not {sema::type}", sema_value_typeof(from));
                return NULL;
            }
            segment->sema.type = SEMA_PATH_DEREF;
            segment->sema.deref_type = sema_value_typeof(from)->ptr_to;
            return sema_value_var(sema_value_typeof(from)->ptr_to);
        }
		case AST_PATH_SEG_IDENT: {
			if (sema_value_typeof(from)->type == SEMA_TYPE_STRUCT) {
                segment->sema.type = SEMA_PATH_STRUCT_MEMBER;
                segment->sema.struct_member.of = sema_value_typeof(from);
                ssize_t idx = segment->sema.struct_member.idx = sema_get_struct_member_id(sema, segment->loc, sema_value_typeof(from)->struct_def, &segment->ident);
                if (idx != -1) {
                    if (sema_value_is_const(from)) {
                        for (size_t i = 0; i < vec_len(from->constant.structure.fields); i++) {
                            SemaConstStructField *field = &from->constant.structure.fields[i];
                            if (field->idx == (size_t)idx) {
                                if (field->is_undefined) {
                                    SEMA_ERROR(segment->loc, "trying to get undefined value");
                                    return NULL;
                                }
                                SemaConst constant;
                                memcpy(&constant, &field->value, sizeof(SemaConst));
                                return sema_value_const(constant); 
                            }
                        }
                    } else {
                        return sema_value_with_type(from, sema_value_typeof(from)->struct_def->members[idx].type->sema);
                    }
                }
			} else if (sema_value_typeof(from)->type == SEMA_TYPE_SLICE) {
                Slice length = slice_from_const_cstr("length");
                Slice raw = slice_from_const_cstr("raw");
                segment->sema.slice_type = sema_value_typeof(from);
                if (slice_eq(&raw, &segment->ident)) {
                    segment->sema.type = SEMA_PATH_SLICE_RAW;
                    SemaType *output_type = sema_type_new_pointer(sema_value_typeof(from)->slice_of);
                    return sema_value_with_type(from, output_type);
                } else if (slice_eq(&length, &segment->ident)) {
                    segment->sema.type = SEMA_PATH_SLICE_LEN;
                    return sema_value_with_type(from, sema_arch_usize(sema));
                }

            } else if (sema_value_typeof(from)->type == SEMA_TYPE_ARRAY) {
                Slice length = slice_from_const_cstr("length");
                if (slice_eq(&length, &segment->ident)) {
                    segment->sema.type = SEMA_PATH_ARRAY_LEN;
                    segment->sema.array_length = sema_value_typeof(from)->array.length;
                    return segment->sema.value = sema_value_final(sema_arch_usize(sema));
                }
            } else if (sema_value_typeof(from)->type == SEMA_TYPE_GENERIC) {
                if (from->sema_type->generic.replace) {
                    return sema_resolve_inner_value_path(sema, sema_value_with_type(from, from->sema_type->generic.replace), segment);
                }
                sema_behaviour_swap_self_type(from->sema_type->generic.behaviour, from->sema_type);
                if ((segment->sema.value = sema_behaviour_resolve(from->sema_type->generic.behaviour, &segment->ident))) {
                    // TODO: specify
                    return segment->sema.value;
                } else {
                    SEMA_ERROR(segment->loc, "there is not member `{slice}` described in generic constraints", &segment->ident);
                    return NULL;
                }
            }
            SemaDecl *decl = sema_module_resolve_ext_func(sema, &segment->ident, sema_value_typeof(from));
            if (decl) {
                segment->sema.type = SEMA_PATH_EXT_FUNC_DIRECT;
                segment->sema.ext_func_decl = decl;
                return sema_value_ext_func_handle(sema_value_typeof(decl->value));
            }
            if (from->type == SEMA_VALUE_VAR) {
                SemaDecl *ptr_decl = sema_module_resolve_ext_func(sema, &segment->ident, sema_type_new_pointer(sema_value_typeof(from)));
                segment->sema.ext_func_decl = ptr_decl;
                if (ptr_decl) {
                    segment->sema.type = SEMA_PATH_EXT_FUNC_REF;
                    return sema_value_ext_func_handle(sema_value_typeof(ptr_decl->value));
                }
            }
            SEMA_ERROR(segment->loc, "{sema::type} has no member `{slice}`", sema_value_typeof(from), &segment->ident);
			return NULL;
		}
	}
    assert(0, "falled through");
	return NULL;
}

SemaDecl *sema_resolve_inner_module_path(SemaModule *sema, SemaModule *from, AstPathSegment *segment) {
    switch (segment->type) {
        case AST_PATH_SEG_GENERIC:
        case AST_PATH_SEG_SIZEOF:
        case AST_PATH_SEG_NULL:
        case AST_PATH_SEG_DEREF:
            SEMA_ERROR(segment->loc, "invalid operation on module");
            break;
        case AST_PATH_SEG_IDENT: {
            SemaDecl *decl = NULL;
            if (sema != from) {
                decl = sema_module_resolve_public_decl(from, &segment->ident);
            } else {
                decl = sema_module_resolve_scope_decl(from, &segment->ident);
            }
            if (!decl) {
                SEMA_ERROR(segment->loc, "`{slice}` is undefined", &segment->ident);
            }
            segment->sema.type = SEMA_PATH_DECL;
            segment->sema.decl = decl;
            return decl;
        }
    }
    assert(0, "falled through");
	return NULL;
}

SemaValue *sema_resolve_inner_generic_path(SemaModule *sema, SemaValue *from, AstPathSegment *segment) {
    switch (segment->type) {
        case AST_PATH_SEG_SIZEOF:
        case AST_PATH_SEG_NULL:
        case AST_PATH_SEG_IDENT:
        case AST_PATH_SEG_DEREF:
            SEMA_ERROR(segment->loc, "cannot get path from generic", sema_value_typeof(from));
	        return NULL;
        case AST_PATH_SEG_GENERIC: {
            size_t got_len = vec_len(segment->generic_params),
                    expected_len = vec_len(from->generic.types);
            if (got_len != expected_len) {
                SEMA_ERROR(segment->loc, "expected {size} params but got {size}", expected_len, got_len);
                return NULL;
            }
            for (size_t i = 0; i < vec_len(segment->generic_params); i++) {
                AstExpr *expr = segment->generic_params[i];
                SemaValue *value = sema_expr(sema, expr, sema_expr_ctx_default());
                if (!value) {
                    return NULL;
                }
                SemaType *type = sema_value_is_type(value);
                if (!type) {
                    SEMA_ERROR(expr->loc, "generic accepts types only");
                    return NULL;
                }
                if (!sema_type_behaves_as(sema, expr->loc, type, from->generic.types[i]->behaviour)) {
                    return NULL;
                }
                from->generic.types[i]->replace = type;
            }
            return sema_value_final(from->generic.target_type);
        }
    }
    assert(0, "falled through");
	return NULL;
}

SemaValue *sema_resolve_inner_type_path(SemaModule *sema, SemaValue *from, AstPathSegment *segment) {
    switch (segment->type) {
        case AST_PATH_SEG_GENERIC:
            SEMA_ERROR(segment->loc, "trying to get generic from type {sema::type}", sema_value_typeof(from));
	        return NULL;
        case AST_PATH_SEG_SIZEOF:
            segment->sema.type = SEMA_PATH_SIZEOF;
            segment->sema.sizeof_op.type = sema_value_typeof(from);
            segment->sema.sizeof_op.output_type = sema_arch_usize(sema);
            // TODO: sizeof is constant
            return sema_value_final(segment->sema.sizeof_op.output_type);
        case AST_PATH_SEG_NULL:
            SEMA_ERROR(segment->loc, "cannot get a member `null` from type `{sema::type}`", sema_value_typeof(from));
	        return NULL;
        case AST_PATH_SEG_IDENT:
            SEMA_ERROR(segment->loc, "cannot get a member `{slice}` from type `{sema::type}`", &segment->ident, sema_value_typeof(from));
	        return NULL;
        case AST_PATH_SEG_DEREF:
            SEMA_ERROR(segment->loc, "cannot dereference type `{sema::type}`", sema_value_typeof(from));
	        return NULL;
    }
    assert(0, "falled through");
	return NULL;
}

SemaValue *sema_resolve_path(SemaModule *sema, SemaValue *from, AstPath *path) {
    if (!from) {
        from = sema_value_module(sema);
    }
    for (size_t i = 0; i < vec_len(path->segments); i++) {
	    AstPathSegment *segment = &path->segments[i];
        segment->sema.value = from;
        switch (from->type) {
            case SEMA_VALUE_TYPE:
                from = sema_resolve_inner_type_path(sema, from, segment);
                if (!from) {
                    return NULL;
                }
                break;
            case SEMA_VALUE_CONST:
            case SEMA_VALUE_FINAL:
            case SEMA_VALUE_VAR: {
                from = sema_resolve_inner_value_path(sema, from, segment);
                if (!from) {
                    return NULL;
                }
                break;
            }
            case SEMA_VALUE_GENERIC: { 
                from = sema_resolve_inner_generic_path(sema, from, segment);
                if (!from) {
                    return NULL;
                }
                break;
            }
            case SEMA_VALUE_BEHAVIOUR:
        	    SEMA_ERROR(segment->loc, "cannot get path from behaviour");
                break;
            case SEMA_VALUE_EXT_FUNC_HANDLE:
        	    SEMA_ERROR(segment->loc, "cannot get path from ext func handle");
                break;
            case SEMA_VALUE_MODULE: {
                SemaDecl *decl = sema_resolve_inner_module_path(sema, from->module, segment);
                if (!decl) {
                    return NULL;
                }
                from = decl->value;
                break;
            }
        }
    }
    return from;
}

SemaDecl *sema_resolve_decl_path(SemaModule *sema, AstPath *path) {
    SemaDecl *decl = NULL;
    SemaModule *module = sema;
    for (size_t i = 0; i < vec_len(path->segments); i++) {
	    AstPathSegment *segment = &path->segments[i];
        if (decl && decl->value->type != SEMA_VALUE_MODULE) {
            SEMA_ERROR(segment->loc, "trying to get decl path not through module");
            return NULL;
        }
        decl = sema_resolve_inner_module_path(sema, module, segment);
        if (!decl) {
            return NULL;
        }
        if (decl->value->type == SEMA_VALUE_MODULE) {
            module = decl->value->module;
        }
    }
    return decl;
}

SemaType *sema_resolve_type_path(SemaModule *sema, SemaValue *from, AstPath *path) {
    SemaValue *value = sema_resolve_path(sema, from, path);
    if (!value) {
        return NULL;
    }
    switch (value->type) {
        case SEMA_VALUE_VAR:
        case SEMA_VALUE_GENERIC:
        case SEMA_VALUE_BEHAVIOUR:
        case SEMA_VALUE_FINAL:
        case SEMA_VALUE_CONST:
        case SEMA_VALUE_MODULE:
        case SEMA_VALUE_EXT_FUNC_HANDLE:
            SEMA_ERROR(path->segments[0].loc, "{ast::path} is not a type", path);
            return NULL;
        case SEMA_VALUE_TYPE:
            break;
    }
    return sema_value_typeof(value);
}

SemaType *sema_resolve_value_path(SemaModule *sema, SemaValue *from, AstPath *path) {
    SemaValue *value = sema_resolve_path(sema, from, path);
    if (!value) {
        return NULL;
    }
    switch (value->type) {
        case SEMA_VALUE_VAR:
        case SEMA_VALUE_CONST:
        case SEMA_VALUE_FINAL:
            break;
        case SEMA_VALUE_TYPE:
        case SEMA_VALUE_MODULE:
        case SEMA_VALUE_EXT_FUNC_HANDLE:
        case SEMA_VALUE_GENERIC:
        case SEMA_VALUE_BEHAVIOUR:
            SEMA_ERROR(path->segments[0].loc, "{ast::path} is not a value", path);
            return false;
    }
    return sema_value_typeof(value);
}
