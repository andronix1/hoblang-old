#include "sema.h"

Sema sema_new() {
	Sema result = {
		.scopes = vec_new(SemaScope),
		.failed = false
	};
	return result;
}

void sema_push_scope(Sema *sema) {
	SemaScope scope = {
		.decls = vec_new(SemaRecord),
		.types = vec_new(SemaRecord)
	};
	sema->scopes = vec_push(sema->scopes, &scope);
}

SemaScope *sema_pop_scope(Sema *sema) {
	return vec_pop(sema->scopes);
}

SemaType *sema_resolve_decl_type(Sema *sema, Slice *name) {
	for (size_t i = 0; i < vec_len(sema->scopes); i++) {
		for (size_t j = 0; j < vec_len(sema->scopes[i].decls); j++) {
			SemaRecord *record = &sema->scopes[i].decls[j];
			if (slice_eq(name, &record->name)) {
				return &record->type;
			}
		}
	}
	return NULL;
}

SemaType *sema_resolve_ident_type(Sema *sema, Slice *name) {
	for (size_t i = 0; i < vec_len(sema->scopes); i++) {
		for (size_t j = 0; j < vec_len(sema->scopes[i].types); j++) {
			SemaRecord *record = &sema->scopes[i].types[j];
			if (slice_eq(name, &record->name)) {
				return &record->type;
			}
		}
	}
	return NULL;
}

void sema_push_type(Sema *sema, SemaRecord *type) {
	if (sema_resolve_ident_type(sema, &type->name)) {
		sema_err("redefinition of type `{slice}`", &type->name);
		return;
	}
	SemaScope *scope = vec_top(sema->scopes);
	scope->types = vec_push(scope->types, type);
}

void sema_push_decl(Sema *sema, SemaRecord *decl) {
	if (sema_resolve_decl_type(sema, &decl->name)) {
		sema_err("redefinition of decl`{slice}`", &decl->name);
		return;
	}
	SemaScope *scope = vec_top(sema->scopes);
	scope->decls = vec_push(scope->decls, decl);
}

void sema_push_primitive(Sema *sema, const char *name, Primitive primitive) {
	SemaRecord record = {
		.name = slice_from_cstr(name),
		.type = { .type = SEMA_TYPE_PRIMITIVE, .primitive = primitive }
	};
	sema_push_type(sema, &record);
}

void sema_push_primitives(Sema *sema) {
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

