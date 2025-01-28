#include "project.h"

SemaModule *sema_project_add_module_at(SemaProject *project, const char *path) {
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
	char *dir, *filename;
	path_split_filename_dir(strdup(path), &dir, &filename);
	char cwd[PATH_MAX];
	getcwd(cwd, PATH_MAX);
	if (dir) {
		chdir(dir);
	}
	if (!lexer_init(&lexer, filename)) {
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
	hob_log(LOGD, "reading module `{cstr}`...", filename);
	SemaModule *sema = sema_module_new(project, module);
	sema->project = project;
	sema_module_read(sema);
	if (sema->failed) {
		chdir(cwd);
		return NULL;
	}
	SemaProjectModule project_module = {
		.module = sema,
		.path = slice_path
	};
	project->modules = vec_push(project->modules, &project_module);
    chdir(cwd);
	return sema;
}

SemaProject *sema_project_new() {
	SemaProject *project = malloc(sizeof(SemaProject));
	project->modules = vec_new(SemaProjectModule);
	return project;
}

bool sema_project_analyze(SemaProject *project) {
	assert(vec_len(project->modules) > 0, "cannot analyze an empty project");
	bool success = true;
	for (size_t i = 0; i < vec_len(project->modules); i++) {
		SemaProjectModule *module = &project->modules[i];
		sema_module(module->module);
		if (module->module->failed) {
			success = false;
		}
	}
	return success;
}