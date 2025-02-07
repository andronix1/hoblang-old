#include "../parts.h"

/*
path - const / var
PATH: var / module / const / type
	MOD_PATH: mod1::mod2::val;
	 |-	TYPE
	 |- MODULE
	 |- CONST
	 |- VAR
	INNER_PATH: 
	 |- TYPE -> const
	 |- MODULE -> !
	 |- CONST -> const		| TYPE OF
	 |- VAR -> var			| SIZE_OF

SemaType *sema_resolve_path_in_type(SemaModule *sema, SemaType *type, AstPath *path, size_t segment_idx) {
	assert(segment_idx < vec_len(path->segments), "trying to resolve invalid path segment");
    if (vec_len(path->segments) == segment_idx) {
        return type;
    }
    AstPathSegment *segment = &path->segments[segment_idx];
    switch (type->type) {
        case SEMA_TYPE_STRUCT:
            AstStructType *struct_type = type->struct_type;
            for (size_t i = 0; i < vec_len(struct_type->members); i++) {
                if (slice_eq(&struct_type->members[i].name, &segment->name)) {
                    segment->sema.type = SEMA_PATH_SEGMENT_STRUCT_MEMBER;
                    segment->sema.struct_member.member_id = i;
                    // segment->sema.struct_member.member_type = struct_type->members[i].type->sema;
                    segment->sema.struct_member.struct_type = type;
                    return sema_resolve_path_in_type(sema, struct_type->members[i].type->sema, path, segment_idx + 1);
                }
            }
            sema_err("segment `{slice}` in `{ast::path}` was not found!", &segment->name, path);
            return NULL;
        case SEMA_TYPE_SLICE:
            Slice length = slice_from_const_cstr("length");
            Slice raw = slice_from_const_cstr("raw");
            if (slice_eq(&length, &segment->name)) {
                segment->sema.type = SEMA_PATH_SEGMENT_SLICE_LENGTH;
                return sema_resolve_path_in_type(sema, &primitives[PRIMITIVE_I32], path, segment_idx + 1);
            } else if (slice_eq(&raw, &segment->name)) {
                segment->sema.type = SEMA_PATH_SEGMENT_SLICE_PTR;
                return sema_resolve_path_in_type(sema, sema_type_new_pointer(type->slice_of), path, segment_idx + 1);
            } else {
                sema_err("segment `{slice}` in `{ast::path}` was not found!", &segment->name, path);
                return NULL;
            }
        default: 
            assert(0, "cannot get segment ");
            return NULL;
    }
}

SemaType *sema_resolve_module_path(SemaModule *sema, AstPath *path, size_t segment_idx) {
	assert(segment_idx < vec_len(path->segments), "trying to resolve invalid path segment({long} <= {long})");
    AstPathSegment *segment = &path->segments[segment_idx];
    SemaScopeDecl *decl = (
        segment_idx == 0 ? 
            sema_module_resolve_scope_decl :
            sema_module_resolve_public_decl
    )(sema, &segment->name);
    if (!decl) {
        sema_err("segment `{slice}` in `{ast::path}` was not found!", &segment->name, path);
        return false;
    }
    bool is_last = vec_len(path->segments) == segment_idx + 1;
	path->decl = decl;
	path->inner_path_start = segment_idx;
    switch (decl->type) {
        case SEMA_SCOPE_DECL_MODULE:
            segment->sema.type = SEMA_PATH_SEGMENT_MODULE;
            segment->sema.module = decl->module;
            if (is_last) {
                sema_err("cannot get a module `{ast::path}`!", path);
                return NULL;
            }
            return sema_resolve_module_path(decl->module, path, segment_idx + 1);
        case SEMA_SCOPE_DECL_TYPE:
            segment->sema.type = SEMA_PATH_SEGMENT_TYPE;
            segment->sema.sema_type = decl->sema_type;
            if (is_last) {
                return decl->sema_type;
            }
            sema_err("`{slice}` from `{ast::path}` is a type, so it has no members!", &segment->name, path);
            return NULL;
        case SEMA_SCOPE_DECL_VALUE:
        	print("DECL!!! {slice}\n", &decl->name);
        	printf("%p\n", decl->value_decl.llvm_value);
            segment->sema.type = SEMA_PATH_SEGMENT_DECL;
            segment->sema.decl = decl;
            if (is_last) {
                return decl->value_decl.type;
            }
            // TODO: IS NOT A CONST
            sema_err("IS NOT A CONST NIY");
            return sema_resolve_path_in_type(sema, decl->value_decl.type, path, segment_idx);
    }
    assert(0, "invalid scope decl type");
    return NULL;
	// SemaModule *current = sema;
	// SemaScopeDecl *decl = NULL;
	// for (size_t i = 0; i < vec_len(path->segments); i++) {
	// 	AstPathSegment *segment = &path->segments[i];
	// 	decl = (i == 0 ? sema_module_resolve_scope_decl : sema_module_resolve_public_decl)(current, &segment->name);
	// 	if (!decl) {
	// 		sema_err("segment `{slice}` in `{ast::path}` was not found!", segment, path);
	// 		return NULL;
	// 	}
	// 	if (i != vec_len(path->segments) - 1) {
	// 		if (decl->type != SEMA_SCOPE_DECL_MODULE) {
	// 			sema_err("segment `{slice}` is not a module!", segment);
	// 			return NULL;
	// 		}
	// 		current = decl->module;
	// 	}
	// }
	// return decl;
}

SemaType *sema_resolve_path_type(SemaModule *sema, AstPath *path) {
    return sema_resolve_module_path(sema, path, 0);
}
*/

SemaScopeDecl *sema_resolve_decl_path(SemaModule *sema, AstDeclPath *path) {
	SemaModule *module = sema;
	SemaScopeDecl *decl = NULL;
	for (size_t i = 0; i < vec_len(path->segments); i++) {
		decl = (i == 0 ?
				sema_module_resolve_scope_decl :
				sema_module_resolve_public_decl
			)(module, &path->segments[i]);
        if (!decl) {
            sema_err("`{slice}` in `{ast::dpath}` was not defined", &path->segments[i], path);
            return NULL;
        }
		if (i + 1 != vec_len(path->segments)) {
			if (decl->type != SEMA_SCOPE_DECL_MODULE) {
				sema_err("`{slice}` in `{ast::dpath}` is not a module", &path->segments[i], path);
				return NULL;
			}
			module = decl->module;
		}
	}
    path->decl = decl;
	assert(decl, "trying to resolve empty decl path");
	return decl;
}

bool sema_resolve_inner_value_path(SemaModule *sema, SemaType *type, AstInnerPath *path, size_t segment_idx, SemaValue *value) {
    if (vec_len(path->segments) == segment_idx) {
        return type;
    }
	AstInnerPathSegment *segment = &path->segments[segment_idx];
	bool is_last = segment_idx + 1 == vec_len(path->segments);
	switch (segment->type) {
        case AST_INNER_PATH_SEG_DEREF: {
            if (type->type != SEMA_TYPE_POINTER) {
                sema_err("only pointers can be dereferenced, not {sema::type}\n", type);
                return false;
            }
            segment->sema.type = SEMA_INNER_PATH_DEREF;
            segment->sema.deref_type = type->ptr_to;
            return sema_value_var(value, type->ptr_to);
        }
		case AST_INNER_PATH_SEG_IDENT: {
			if (type->type == SEMA_TYPE_STRUCT) {
				for (size_t i = 0; i < vec_len(type->struct_type->members); i++) {
                    AstStructMember *member = &type->struct_type->members[i];
					if (slice_eq(&member->name, &segment->ident)) {
                        segment->sema.type = SEMA_INNER_PATH_STRUCT_MEMBER;
                        segment->sema.struct_member.idx = i;
                        segment->sema.struct_member.of = type;
                        SemaType *result = member->type->sema;
						if (is_last) {
							return sema_value_var(value, result);
						}
                        return sema_resolve_inner_value_path(sema, result, path, segment_idx + 1, value);
					}
				}
                sema_err("{sema::type} has not a member {slice}", type, &segment->ident);
                return false;
			} else if (type->type == SEMA_TYPE_SLICE) {
                Slice length = slice_from_const_cstr("length");
                Slice raw = slice_from_const_cstr("raw");
                segment->sema.slice_type = type;
                if (slice_eq(&raw, &segment->ident)) {
                    segment->sema.type = SEMA_INNER_PATH_SLICE_LEN;
                    return sema_value_var(value, sema_type_new_pointer(type->slice_of));
                } else if (slice_eq(&length, &segment->ident)) {
                    segment->sema.type = SEMA_INNER_PATH_SLICE_RAW;
                    return sema_value_var(value, &primitives[PRIMITIVE_I32]);
                } else {
                    sema_err("{sema::type} has not member {slice}", type, &segment->ident);
                    return false;
                }
            }
			return false;
		}
	}
	return false;
}

bool sema_resolve_inner_type_path(SemaModule *sema, SemaType *type, AstInnerPath *path, size_t segment_idx, SemaValue *value) {
    if (vec_len(path->segments) == segment_idx) {
        return type;
    }
	bool is_last = segment_idx + 1 == vec_len(path->segments);
	AstInnerPathSegment *segment = &path->segments[segment_idx];
	switch (segment->type) {
		case AST_INNER_PATH_SEG_IDENT:
        	sema_err("cannot get a member `{slice}` from type `{sema::type}`", &segment->ident, type);
			break;
		case AST_INNER_PATH_SEG_DEREF:
        	sema_err("cannot dereference type `{sema::type}`", type);
			break;
	}
	return false;
}

bool sema_resolve_path(SemaModule *sema, AstPath *path, SemaValue *value) {
    SemaScopeDecl *decl = sema_resolve_decl_path(sema, &path->decl_path);
    if (!decl) {
        return false;
    }
    switch (decl->type) {
        case SEMA_SCOPE_DECL_MODULE:
            sema_err("cannot get members of module {ast::dpath}", &path->decl_path);
            return false;
        case SEMA_SCOPE_DECL_TYPE:
            sema_value_type(value, decl->sema_type);
            if (!sema_resolve_inner_type_path(sema, decl->sema_type, &path->inner_path, 0, value)) {
                return false;
            }
            break;
        case SEMA_SCOPE_DECL_VALUE:
            value->type = decl->value_decl.constant ?
                SEMA_VALUE_CONST :
                SEMA_VALUE_VAR;
            value->sema_type = decl->value_decl.type;
            if (!sema_resolve_inner_value_path(sema, decl->value_decl.type, &path->inner_path, 0, value)) {
                return false;
            }
            break;
    }
    return true;
}

SemaType *sema_resolve_type_path(SemaModule *sema, AstPath *path) {
    path->value.sema_type = NULL;
    if (!sema_resolve_path(sema, path, &path->value)) {
        return NULL;
    }
    switch (path->value.type) {
        case SEMA_VALUE_VAR:
        case SEMA_VALUE_CONST:
        case SEMA_VALUE_MODULE:
            sema_err("{ast::path} is value, not type", path);
            return NULL;
        case SEMA_VALUE_TYPE:
            break;
    }
    return path->value.sema_type;
}
SemaType *sema_resolve_value_path(SemaModule *sema, AstPath *path) {
    if (!sema_resolve_path(sema, path, &path->value)) {
        return false;
    }
    switch (path->value.type) {
        case SEMA_VALUE_VAR:
        case SEMA_VALUE_CONST:
            break;
        case SEMA_VALUE_TYPE:
        case SEMA_VALUE_MODULE:
            sema_err("{ast::path} is type, not value", path);
            return false;
    }
    return path->value.sema_type;
}
