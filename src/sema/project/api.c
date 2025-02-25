#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include "core/path.h"
#include "parser/api.h"
#include "sema/module/api.h"
#include "ast/private/module.h"
#include "sema/project/impl.h"
#include "sema/project/api.h"

SemaModule *sema_project_add_module_at(SemaProject *project, const char *path) {
    const char *full_path = realpath(path, NULL);
	if (!full_path) {
		hob_log(LOGE, "cannot resolve path {cstr}", path);
		return NULL;
	}
	Slice slice_path = slice_from_cstr(full_path);
	for (size_t i = 0; i < vec_len(project->modules); i++) {
        Slice path = sema_project_module_path(project->modules[i]);
		if (slice_eq(&slice_path, &path)) {
			return sema_project_module_inner(project->modules[i]);
		}
	}
	char *dir, *filename;
	path_split_filename_dir(strdup(path), &dir, &filename);
	char cwd[PATH_MAX];
	getcwd(cwd, PATH_MAX);
	if (dir) {
		chdir(dir);
	}
	Parser *parser = parser_from_file(full_path);
	if (!parser) {
		chdir(cwd);
		return NULL;
	}
	AstModule *module = malloc(sizeof(AstModule));
    module->path = full_path;
    module->parser = parser;
	parse_module(parser, module);
	if (parser_failed(parser)) {
		chdir(cwd);
		return NULL;
	}
    SemaModule *sema = sema_module_new(project, module);
	sema_module_read_decls(sema);
	if (sema_module_failed(sema)) {
		chdir(cwd);
		return NULL;
	}
    // TODO: constructor
	SemaProjectModule *project_module = malloc(sizeof(SemaProjectModule));
    project_module->module = sema;
	project_module->path = slice_path;
	project->modules = vec_push(project->modules, &project_module);
    chdir(cwd);
	return sema;
}

SemaProject *sema_project_new() {
	SemaProject *project = malloc(sizeof(SemaProject));
	project->modules = vec_new(SemaProjectModule*);
	return project;
}

bool sema_project_analyze(SemaProject *project) {
	assert(vec_len(project->modules) > 0, "cannot analyze an empty project");
	bool success = true;
	for (size_t i = 0; i < vec_len(project->modules); i++) {
		SemaProjectModule *module = project->modules[i];
		sema_module_analyze(module->module);
		if (sema_module_failed(module->module)) {
			success = false;
		}
	}
	return success;
}

SemaProjectModule **sema_project_modules(SemaProject *project) {
    return project->modules;
}

SemaModule *sema_project_module_inner(SemaProjectModule *module) {
    return module->module;
}

Slice sema_project_module_path(SemaProjectModule *module) {
    return module->path;
}
