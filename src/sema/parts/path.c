#include "../parts.h"

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