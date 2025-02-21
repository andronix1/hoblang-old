#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "print.h"
#include "lexer/api.h"
#include "parser/api.h"
#include "sema/project/api.h"
#include "sema/module/api.h"
#include "llvm/api.h"

char *args_shift(int *argc, char ***argv) {
    *argc -= 1;
    char *cached = (*argv)[0];
    *argv = &(*argv)[1];
    return cached;
}

void usage() {
	printf("hoblang compile <source> <output> [dump <output>] - compile source file to machine code\n");
}


int main(int argc, char **argv) {
	print_setup();

	if (argc != 4 && !(argc == 6 && !strcmp(argv[4], "dump"))) {
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
		LlvmBackend *llvm = llvm_create();
		if (!llvm) {
			return 1;
		}
        SemaProjectModule **modules = sema_project_modules(project);
		for (size_t i = 0; i < vec_len(modules); i++) {
            Slice path = sema_project_module_path(modules[i]);
			hob_log(LOGD, "compiling {slice}", &path);
			llvm_module_init(llvm, sema_module_of(sema_project_module_inner(modules[i])));
		}
		for (size_t i = 0; i < vec_len(modules); i++) {
            Slice path = sema_project_module_path(modules[i]);
			hob_log(LOGD, "compiling {slice}", &path);
			llvm_module(llvm, sema_module_of(sema_project_module_inner(modules[i])));
		}
        if (argc > 0) {
            args_shift(&argc, &argv);
		    llvm_write_module_ir(llvm, args_shift(&argc, &argv));
        }
		llvm_write_module(llvm, output_path);
		hob_log(LOGI, "finished!");
    } else {
		usage();
    }
    return 0;
}
