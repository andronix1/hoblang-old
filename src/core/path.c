#include "core/path.h"
#include <stddef.h>
#include <string.h>

void path_split_filename_dir(char *source, char **dir, char **filename) {
	*filename = source;
	*dir = NULL;
	for (int i = strlen(source) - 1; i >= 0; i--) {
		if (source[i] == '/') {
			source[i] = '\0';
			*dir = source;
			*filename = &source[i + 1];
			break;
		}
	}
}
