#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "print.h"
#include "lexer/api.h"
#include "parser/api.h"
#include "sema/api.h"
#include "llvm/parts.h"

char *args_shift(int *argc, char ***argv) {
    *argc -= 1;
    char *cached = (*argv)[0];
    *argv = &(*argv)[1];
    return cached;
}

void usage() {
	printf("hoblang compile <source> <output> - compile source file to machine code\n");
}

int main(int argc, char **argv) {
	print_setup();

	if (argc != 4) {
		usage();
		return 0;
	}
    args_shift(&argc, &argv);
    const char *command = args_shift(&argc, &argv);
    if (!strcmp(command, "compile")) {
		const char *src_path = args_shift(&argc, &argv);
		char *output_path = args_shift(&argc, &argv);
		SemaProject *project = sema_project_new();
		sema_project_add_module_at(project, src_path);
		if (!sema_project_analyze(project)) {
			return 1;
		}
		hob_log(LOGD, "analyzed successfully!");
		LlvmBackend llvm;
		if (!llvm_init(&llvm)) {
			return 1;
		}
		for (size_t i = 0; i < vec_len(project->modules); i++) {
			hob_log(LOGD, "compiling {slice}", &project->modules[i].path);
			llvm_module_init(&llvm, sema_module_of(project->modules[i].module));
		}
		for (size_t i = 0; i < vec_len(project->modules); i++) {
			hob_log(LOGD, "compiling {slice}", &project->modules[i].path);
			llvm_module(&llvm, sema_module_of(project->modules[i].module));
		}
		llvm_write_module(&llvm, output_path);
		hob_log(LOGI, "finished!");
    } else {
		usage();
    }
    return 0;
}
