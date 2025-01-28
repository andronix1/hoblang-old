#include "../parts.h"

SemaScopeDecl *sema_resolve_mod_path(SemaModule *sema, AstModPath *path) {
	assert(vec_len(path->segments), "trying to resolve empty mod path");
	SemaModule *current = sema;
	SemaScopeDecl *decl = NULL;
	for (size_t i = 0; i < vec_len(path->segments); i++) {
		Slice *segment = &path->segments[i];
		decl = (i == 0 ? sema_module_resolve_scope_decl : sema_module_resolve_public_decl)(current, segment);
		if (!decl) {
			sema_err("segment `{slice}` in `{ast::path}` was not found!", segment, path);
			return NULL;
		}
		if (i != vec_len(path->segments) - 1) {
			if (decl->type != SEMA_SCOPE_DECL_MODULE) {
				sema_err("segment `{slice}` is not a module!", segment);
				return NULL;
			}
			current = decl->module;
		}
	}
	return decl;
}

SemaModule *sema_resolve_mod_path_module(SemaModule *sema, AstModPath *path) {
	SemaScopeDecl *decl = sema_resolve_mod_path(sema, path);
	if (!decl) {
		return NULL;
	}
	if (decl->type != SEMA_SCOPE_DECL_MODULE) {
		sema_err("`{ast::path}` is not a module!", path);
		return NULL;
	}
	path->module = decl->module;
	return decl->module;
}

SemaScopeValueDecl *sema_resolve_mod_path_value(SemaModule *sema, AstModPath *path) {
	SemaScopeDecl *decl = sema_resolve_mod_path(sema, path);
	if (!decl) {
		return NULL;
	}
	if (decl->type != SEMA_SCOPE_DECL_VALUE) {
		sema_err("`{ast::path}` is not a value!", path);
		return NULL;
	}
	path->value = &decl->value_decl;
	return &decl->value_decl;
}

SemaType *sema_resolve_mod_path_type(SemaModule *sema, AstModPath *path) {
	SemaScopeDecl *decl = sema_resolve_mod_path(sema, path);
	if (!decl) {
		return NULL;
	}
	if (decl->type != SEMA_SCOPE_DECL_TYPE) {
		sema_err("`{ast::path}` is not a type!", path);
		return NULL;
	}
	path->type = decl->sema_type;
	return decl->sema_type;
}