#include "../parts.h"

SemaTypeDecl *sema_resolve_type_path(SemaModule *sema, AstModPath *path) {
	SemaModule *result = sema;
	for (size_t i = 0; i < vec_len(path->segments) - 1; i++) {
		result = sema_resolve_module(result, &path->segments[i]);
		if (!result) {
			sema_err("module {slice} was not found", &path->segments[i]);
			return NULL;
		}
	}
	if (!(path->type = sema_resolve_type_decl(result, vec_top(path->segments)))) {
		sema_err("type {slice} not found!", vec_top(path->segments));
		return NULL;
	}
	return path->type;
}

SemaValueDecl *sema_resolve_value_path(SemaModule *sema, AstModPath *path) {
	SemaModule *result = sema;
	for (size_t i = 0; i < vec_len(path->segments) - 1; i++) {
		result = sema_resolve_module(result, &path->segments[i]);
		if (!result) {
			sema_err("module {slice} was not found", &path->segments[i]);
			return NULL;
		}
	}
	if (!(path->value = sema_resolve_value_decl(result, vec_top(path->segments)))) {
		sema_err("value {slice} not found!", vec_top(path->segments));
		return NULL;
	}
	return path->value;
}

SemaModule *sema_resolve_module_path(SemaModule *sema, AstModPath *path) {
	SemaModule *result = sema;
	for (size_t i = 0; i < vec_len(path->segments); i++) {
		result = sema_resolve_module(result, &path->segments[i]);
		if (!result) {
			sema_err("module {slice} was not found", &path->segments[i]);
			return NULL;
		}
	}
	return path->module = result;
}
