#include "project.h"
#include "core/path.h"
#include "sema/interface/module.h"
#include "sema/module.h"
#include "core/slice/api.h"
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>

SemaModule *sema_project_append_module_at(SemaProject *project, const char *path) {
    const char *full_path = realpath(path, NULL);
	if (!full_path) {
		hob_log(LOGE, "cannot resolve path {cstr}", path);
		return NULL;
	}
	Slice slice_path = slice_from_cstr(full_path);
	for (size_t i = 0; i < vec_len(project->modules); i++) {
        Slice path = project->modules[i].path;
		if (slice_eq(&slice_path, &path)) {
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
    SemaModule *module = sema_module_from_file(full_path, project);
    if (!module) {
		chdir(cwd);
		return NULL;
    }
    project->modules = vec_push_dir(project->modules, sema_project_module_new(slice_path, module));
    sema_module_read(module);
    chdir(cwd);
    return module;
}
