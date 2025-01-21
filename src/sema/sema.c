#include "sema.h"

SemaModule *sema_new_module(AstModule *module) {
	SemaModule *result = malloc(sizeof(SemaModule));
	result->scopes = vec_new(SemaScope);
	result->modules = vec_new(SemaModuleUsage);
	result->failed = false;
	result->ast = module;
	return result;
}

void sema_push_scope(SemaModule *sema) {
	SemaScope scope = {
		.decls = vec_new(SemaValueDecl*),
		.defers = vec_new(AstDefer*),
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

void sema_push_defer(SemaModule *sema, AstDefer *defer) {
	SemaScope *scope = vec_top(sema->scopes);
	scope->defers = vec_push(scope->defers, &defer);
}

AstDefer **sema_resolve_defers(SemaModule *sema) {
	AstDefer **result = vec_new(AstDefer*);
	for (int i = vec_len(sema->scopes) - 1; i >= 0; i--) {
		SemaScope *scope = &sema->scopes[i];
		for (int j = vec_len(scope->defers) - 1; j >= 0; j--) {
			result = vec_push(result, &scope->defers[j]);
		}
	}
	return result;
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
	return scope->decls[vec_len(scope->decls) - 1];
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

SemaProject *sema_project_new() {
	SemaProject *result = malloc(sizeof(SemaProject));
	result->modules = vec_new(SemaImportedModule);
	return result;
}

SemaModule *sema_project_add_module(SemaProject *project, const char *path) {
	const char *full_path = realpath(path, NULL);
	if (!full_path) {
		hob_log(LOGE, "cannot resolve path {cstr}", path);
		return NULL;
	}
	Slice slice_path = slice_from_cstr(full_path);
	for (size_t i = 0; i < vec_len(project->modules); i++) {
		if (slice_eq(&slice_path, &project->modules[i].path)) {
			return project->modules[i].module;
		}
	}
	Lexer lexer;

	char *pathc = strdup(path);
	char *fname = pathc;
	char *dirname = NULL;
	for (int i = strlen(pathc) - 1; i >= 0; i--) {
		if (pathc[i] == '/') {
			pathc[i] = '\0';
			dirname = pathc;
			fname = &pathc[i + 1];
			break;
		}
	}

	char cwd[PATH_MAX];
	getcwd(cwd, PATH_MAX);
	if (dirname) {
		chdir(dirname);
	}
	if (!lexer_init(&lexer, fname)) {
		chdir(cwd);
		return NULL;
	}
	Parser parser;
	if (!parser_init(&parser, &lexer)) {
		chdir(cwd);
		return NULL;
	}
	AstModule *module = malloc(sizeof(AstModule));
	parse_module(&parser, module);
	if (lexer.failed || parser.failed) {
		chdir(cwd);
		return NULL;
	}
	SemaModule *sema = sema_new_module(module);
	sema->project = project;
	sema_module_read(sema);
	if (sema->failed) {
		chdir(cwd);
		return NULL;
	}
	hob_log(LOGD, "module `{cstr}` readed successfully!", fname);
	SemaImportedModule imported_module = {
		.module = sema,
		.path = slice_path
	};
	project->modules = vec_push(project->modules, &imported_module);
		chdir(cwd);
	return sema;
}

SemaModule *sema_resolve_module(SemaModule *sema, Slice *name) {
	for (size_t j = 0; j < vec_len(sema->modules); j++) {
		SemaModuleUsage *module = &sema->modules[j];
		if (slice_eq(name, &module->name)) {
			return module->module;
		}
	}
	return NULL;
}

bool sema_push_module_usage(SemaModule *sema, Slice name, SemaModule *module) {
	if (sema_resolve_module(sema, &name)) {
		sema_err("module `{slice}` is already used", &name);
		return false;
	}
	SemaModuleUsage mod = {
		.module = module,
		.name = name
	};
	sema->modules = vec_push(sema->modules, &mod);
	return true;
}

bool sema_project(SemaProject *project) {
	bool failed = false;
	for (size_t i = 0; i < vec_len(project->modules); i++) {
		SemaModule *sema = project->modules[i].module;
		sema_module(sema);
		if (sema->failed) {
			failed = true;
		}
	}
	return !failed;//*(SemaModule**)vec_top(project->modules);
}
