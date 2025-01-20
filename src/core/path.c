#include "path.h"

char *realpath_rel(const char *path, const char *wd) {
	char cwd[PATH_MAX];
	getcwd(cwd, PATH_MAX);
	chdir(wd);
	char *result = realpath(path, NULL);
	chdir(cwd);
	return result;
}
