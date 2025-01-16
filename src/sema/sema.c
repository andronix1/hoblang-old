#include "sema.h"

SemaModule sema_new_module(AstModule *module) {
	SemaModule result = {
		.scopes = vec_new(SemaScope),
		.failed = false,
		.ast = module
	};
	return result;
}

void sema_push_scope(SemaModule *sema) {
	SemaScope scope = {
		.decls = vec_new(SemaValueDecl*),
		.types = vec_new(SemaTypeDecl*)
	};
	assert(scope.decls, "a");
	assert(scope.types, "o");
	sema->scopes = vec_push(sema->scopes, &scope);
}

SemaScope *sema_pop_scope(SemaModule *sema) {
	return vec_pop(sema->scopes);
}

SemaValueDecl *sema_resolve_value_decl(SemaModule *sema, Slice *name) {
	for (size_t i = 0; i < vec_len(sema->scopes); i++) {
		for (size_t j = 0; j < vec_len(sema->scopes[i].decls); j++) {
			SemaValueDecl *decl = sema->scopes[i].decls[j];
			if (slice_eq(name, &decl->name)) {
				return decl;
			}
		}
	}
	return NULL;
}

SemaType *sema_resolve_decl_type(SemaModule *sema, Slice *name) {
	SemaValueDecl *decl = sema_resolve_value_decl(sema, name);
	if (!decl) {
		return NULL;
	}
	return decl->type;
}

SemaType *sema_resolve_ident_type(SemaModule *sema, Slice *name) {
	for (size_t i = 0; i < vec_len(sema->scopes); i++) {
		for (size_t j = 0; j < vec_len(sema->scopes[i].types); j++) {
			SemaTypeDecl *type = sema->scopes[i].types[j];
			if (slice_eq(name, &type->name)) {
				return type->type;
			}
		}
	}
	return NULL;
}

void sema_push_type(SemaModule *sema, Slice name, SemaType *type) {
	if (sema_resolve_ident_type(sema, &name)) {
		sema_err("redefinition of type `{slice}`", &name);
		return;
	}
	SemaScope *scope = vec_top(sema->scopes);
	SemaValueDecl *data = malloc(sizeof(SemaValueDecl));
	data->name = name;
	data->type = type;
	scope->types = vec_push(scope->types, &data);
}

SemaValueDecl *sema_push_decl(SemaModule *sema, Slice name, SemaType *type) {
	if (sema_resolve_decl_type(sema, &name)) {
		sema_err("redefinition of decl`{slice}`", &name);
		return NULL;
	}
	SemaScope *scope = vec_top(sema->scopes);
	SemaValueDecl *data = malloc(sizeof(SemaValueDecl));
	data->type = type;
	data->name = name;
	data->llvm_value = NULL;
	scope->decls = vec_push(scope->decls, &data);
	return vec_top(scope->decls);
}

void sema_push_primitive(SemaModule *sema, const char *name, Primitive primitive) {
	sema_push_type(sema, slice_from_cstr(name), &primitives[primitive]);
}

void sema_push_primitives(SemaModule *sema) {
	sema_push_primitive(sema, "void", PRIMITIVE_VOID);
	sema_push_primitive(sema, "bool", PRIMITIVE_BOOL);
	sema_push_primitive(sema, "i8", PRIMITIVE_I8);
	sema_push_primitive(sema, "i16", PRIMITIVE_I16);
	sema_push_primitive(sema, "i32", PRIMITIVE_I32);
	sema_push_primitive(sema, "i64", PRIMITIVE_I64);
	sema_push_primitive(sema, "u8", PRIMITIVE_U8);
	sema_push_primitive(sema, "u16", PRIMITIVE_U16);
	sema_push_primitive(sema, "u32", PRIMITIVE_U32);
	sema_push_primitive(sema, "u64", PRIMITIVE_U64);
}

