#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "print.h"
#include "lexer/lex.h"
#include "parser/parsers.h"
#include "sema/parts.h"
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
        Lexer lexer;
		if (!lexer_init(&lexer, src_path)) {
			return 1;
		}
		Parser parser;
		if (!parser_init(&parser, &lexer)) {
			return 1;
		}
		AstModule module;
		parse_module(&parser, &module);
		if (lexer.failed || parser.failed) {
			return 1;
		}
		hob_log(LOGD, "parsed successfully!");
		SemaModule sema = sema_new_module(&module);
		sema_module_read(&sema);
		sema_module(&sema);
		if (sema.failed) {
			return 1;
		}
		hob_log(LOGD, "analyzed successfully!");
		LlvmBackend llvm;
		if (!llvm_init(&llvm)) {
			return 1;
		}
		llvm_module(&llvm, &module);
		llvm_write_module(&llvm, output_path);
		hob_log(LOGI, "finished!");
    } else {
		usage();
    }
    return 0;
}
