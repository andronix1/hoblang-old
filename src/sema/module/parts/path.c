#include "ast/private/path.h"
#include "ast/private/type.h"
#include "ast/private/module_node.h"
#include "core/vec.h"
#include "sema/arch/bits/private.h"
#include "sema/module/module.h"
#include "sema/type/private.h"
#include "sema/module/private.h"
#include "sema/module/decls/impl.h"
#include "sema/module/parts/decls/struct/api.h"
#include "sema/module/parts/decls/struct/impl.h"
#include "sema/value/private.h"
#include "sema/value/api.h"

SemaValue *sema_resolve_inner_value_path(SemaModule *sema, SemaValue *from, AstPathSegment *segment) {
    segment->sema.value = from;
	switch (segment->type) {
        case AST_PATH_SEG_NULL: {
            if (from->sema_type->type != SEMA_TYPE_OPTIONAL) {
                SEMA_ERROR(segment->loc, "cannot determine is value null for type {sema::type}", from->sema_type);
                return NULL;
            }
            segment->sema.type = SEMA_PATH_IS_NULL;
            segment->sema.optional_type = from->sema_type;
            return sema_value_with_type(from, sema_type_primitive_bool());
        }
        case AST_PATH_SEG_SIZEOF: {
            segment->sema.type = SEMA_PATH_SIZEOF;
            segment->sema.sizeof_op.type = from->sema_type;
            segment->sema.sizeof_op.output_type = sema_arch_usize(sema);
            // TODO: sizeof is constant
            return sema_value_final(segment->sema.sizeof_op.output_type);
        }
        case AST_PATH_SEG_DEREF: {
            if (from->sema_type->type != SEMA_TYPE_POINTER) {
                SEMA_ERROR(segment->loc, "only pointers can be dereferenced, not {sema::type}", from->sema_type);
                return NULL;
            }
            segment->sema.type = SEMA_PATH_DEREF;
            segment->sema.deref_type = from->sema_type->ptr_to;
            return sema_value_var(from->sema_type->ptr_to);
        }
		case AST_PATH_SEG_IDENT: {
			if (from->sema_type->type == SEMA_TYPE_STRUCT) {
                segment->sema.type = SEMA_PATH_STRUCT_MEMBER;
                segment->sema.struct_member.of = from->sema_type;
                SemaStructMember *member = segment->sema.struct_member.member = sema_get_struct_member(sema, segment->loc, from->sema_type->struct_def, &segment->ident);
                if (!member) {
                    return NULL;
                }
                switch (member->type) {
                    case SEMA_STRUCT_MEMBER_FIELD:
                        return sema_value_with_type(from, from->sema_type->struct_def->members[member->field_idx].type->sema);
                    case SEMA_STRUCT_MEMBER_EXT_FUNC:
                        return sema_value_ext_func_handle(sema_value_typeof(member->ext_func.decl->value));
                }
                assert(0, "invalid struct member type");
			} else if (from->sema_type->type == SEMA_TYPE_SLICE) {
                Slice length = slice_from_const_cstr("length");
                Slice raw = slice_from_const_cstr("raw");
                segment->sema.slice_type = from->sema_type;
                if (slice_eq(&raw, &segment->ident)) {
                    segment->sema.type = SEMA_PATH_SLICE_RAW;
                    SemaType *output_type = sema_type_new_pointer(from->sema_type->slice_of);
                    return sema_value_with_type(from, output_type);
                } else if (slice_eq(&length, &segment->ident)) {
                    segment->sema.type = SEMA_PATH_SLICE_LEN;
                    return sema_value_with_type(from, sema_arch_usize(sema));
                } else {
                    SEMA_ERROR(segment->loc, "{sema::type} has not member {slice}", from->sema_type, &segment->ident);
                    return NULL;
                }
            } else if (from->sema_type->type == SEMA_TYPE_ARRAY) {                Slice length = slice_from_const_cstr("length");
                Slice raw = slice_from_const_cstr("raw");
                if (slice_eq(&length, &segment->ident)) {
                    segment->sema.type = SEMA_PATH_ARRAY_LEN;
                    segment->sema.array_length = from->sema_type->array.length;
                    return sema_value_final(sema_arch_usize(sema));
                } else {
                    SEMA_ERROR(segment->loc, "{sema::type} has not member {slice}", from->sema_type, &segment->ident);
                    return NULL;
                }
            }
			return NULL;
		}
	}
    assert(0, "falled through");
	return NULL;
}

SemaDecl *sema_resolve_inner_module_path(SemaModule *sema, SemaModule *from, AstPathSegment *segment) {
    switch (segment->type) {
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

SemaValue *sema_resolve_inner_type_path(SemaModule *sema, SemaValue *from, AstPathSegment *segment) {
    switch (segment->type) {
        case AST_PATH_SEG_SIZEOF:
            segment->sema.type = SEMA_PATH_SIZEOF;
            segment->sema.sizeof_op.type = from->sema_type;
            segment->sema.sizeof_op.output_type = sema_arch_usize(sema);
            // TODO: sizeof is constant
            return sema_value_final(segment->sema.sizeof_op.output_type);
        case AST_PATH_SEG_NULL:
            SEMA_ERROR(segment->loc, "cannot get a member `null` from type `{sema::type}`", from->sema_type);
            break;
        case AST_PATH_SEG_IDENT:
            SEMA_ERROR(segment->loc, "cannot get a member `{slice}` from type `{sema::type}`", &segment->ident, from->sema_type);
            break;
        case AST_PATH_SEG_DEREF:
            SEMA_ERROR(segment->loc, "cannot dereference type `{sema::type}`", from->sema_type);
            break;
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
            SEMA_ERROR(path->segments[0].loc, "{ast::path} is not a value", path);
            return false;
    }
    return sema_value_typeof(value);
}
