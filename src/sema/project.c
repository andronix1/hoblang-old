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
	char *dir, *filename;
	path_split_filename_dir(strdup(path), &dir, &filename);
	char cwd[PATH_MAX];
	getcwd(cwd, PATH_MAX);
	if (dir) {
		chdir(dir);
	}
	Parser *parser = parser_from_file(filename);
	if (!parser) {
		chdir(cwd);
		return NULL;
	}
	AstModule *module = malloc(sizeof(AstModule));
	parse_module(parser, module);
	if (parser_failed(parser)) {
		chdir(cwd);
		return NULL;
	}
	hob_log(LOGD, "reading module `{cstr}`...", filename);
	SemaModule *sema = sema_module_new(project, module);
    //sema->project = project;
	sema_module_read_decls(sema);
	if (sema_module_failed(sema)) {
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
		hob_log(LOGD, "analyzing module `{slice}`...", &module->path);
		sema_module_analyze(module->module);
		if (sema_module_failed(module->module)) {
			success = false;
		}
	}
	return success;
}
